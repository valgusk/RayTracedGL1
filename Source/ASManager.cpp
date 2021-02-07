// Copyright (c) 2020-2021 Sultim Tsyrendashiev
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "ASManager.h"

#include <array>

#include "Utils.h"
#include "Generated/ShaderCommonC.h"

ASManager::ASManager(
    VkDevice _device, 
    std::shared_ptr<MemoryAllocator> _allocator,
    std::shared_ptr<CommandBufferManager> _cmdManager,
    std::shared_ptr<TextureManager> _textureMgr,
    const VertexBufferProperties &_properties)
:
    device(_device),
    allocator(std::move(_allocator)),
    cmdManager(std::move(_cmdManager)),
    textureMgr(std::move(_textureMgr)),
    properties(_properties)
{
    typedef VertexCollectorFilterTypeFlags FL;
    typedef VertexCollectorFilterTypeFlagBits FT;

    // init AS structs for each dimension
    for (auto cf : VertexCollectorFilterGroup_ChangeFrequency)
    {
        for (auto pt : VertexCollectorFilterGroup_PassThrough)
        {
            auto filter = cf | pt;

            if (filter & FT::CF_DYNAMIC)
            {
                for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
                {
                    allDynamicBlas[i].emplace_back(filter);
                }
            }
            else
            {
                allStaticBlas.emplace_back(filter);
            }
        }
    }

    scratchBuffer = std::make_shared<ScratchBuffer>(allocator);
    asBuilder = std::make_shared<ASBuilder>(device, scratchBuffer);

    // static and movable static vertices share the same buffer as their data won't be changing
    collectorStatic = std::make_shared<VertexCollector>(
        device, allocator,
        sizeof(ShVertexBufferStatic), properties,
        FT::CF_STATIC_NON_MOVABLE | FT::CF_STATIC_MOVABLE | FT::MASK_PASS_THROUGH_GROUP);

    // subscribe to texture manager only static collector,
    // as static geometries aren't updating its material info (in ShGeometryInstance)
    // every frame unlike dynamic ones
    textureMgr->Subscribe(collectorStatic);

    // dynamic vertices
    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        collectorDynamic[i] = std::make_shared<VertexCollector>(
            device, allocator,
            sizeof(ShVertexBufferDynamic), properties,
            FT::CF_DYNAMIC | FT::MASK_PASS_THROUGH_GROUP);
    }

    // instance buffer for TLAS
    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        instanceBuffers[i].Init(
            allocator,
            MAX_TOP_LEVEL_INSTANCE_COUNT * sizeof(VkAccelerationStructureInstanceKHR),
            VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            "TLAS instance buffer");
    }

    CreateDescriptors();

    // buffers won't be changing, update once
    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        UpdateBufferDescriptors(i);
    }

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = 0;
    VkResult r = vkCreateFence(device, &fenceInfo, nullptr, &staticCopyFence);
    VK_CHECKERROR(r);

    SET_DEBUG_NAME(device, staticCopyFence, VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT, "Static BLAS fence");
}

#pragma region AS descriptors

void ASManager::CreateDescriptors()
{
    VkResult r;

    {
        std::array<VkDescriptorSetLayoutBinding, 6> bindings{};

        // static vertex data
        bindings[0].binding = BINDING_VERTEX_BUFFER_STATIC;
        bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[0].descriptorCount = 1;
        bindings[0].stageFlags = VK_SHADER_STAGE_ALL;

        // dynamic vertex data
        bindings[1].binding = BINDING_VERTEX_BUFFER_DYNAMIC;
        bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[1].descriptorCount = 1;
        bindings[1].stageFlags = VK_SHADER_STAGE_ALL;

        bindings[2].binding = BINDING_INDEX_BUFFER_STATIC;
        bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[2].descriptorCount = 1;
        bindings[2].stageFlags = VK_SHADER_STAGE_ALL;

        bindings[3].binding = BINDING_INDEX_BUFFER_DYNAMIC;
        bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[3].descriptorCount = 1;
        bindings[3].stageFlags = VK_SHADER_STAGE_ALL;

        bindings[4].binding = BINDING_GEOMETRY_INSTANCES_STATIC;
        bindings[4].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[4].descriptorCount = 1;
        bindings[4].stageFlags = VK_SHADER_STAGE_ALL;

        bindings[5].binding = BINDING_GEOMETRY_INSTANCES_DYNAMIC;
        bindings[5].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[5].descriptorCount = 1;
        bindings[5].stageFlags = VK_SHADER_STAGE_ALL;

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = bindings.size();
        layoutInfo.pBindings = bindings.data();

        r = vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &buffersDescSetLayout);
        VK_CHECKERROR(r);
    }

    {
        std::array<VkDescriptorSetLayoutBinding, 1> bindings{};

        bindings[0].binding = BINDING_ACCELERATION_STRUCTURE;
        bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
        bindings[0].descriptorCount = 1;
        bindings[0].stageFlags = VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = bindings.size();
        layoutInfo.pBindings = bindings.data();

        r = vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &asDescSetLayout);
        VK_CHECKERROR(r);
    }

    std::array<VkDescriptorPoolSize, 2> poolSizes{};

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[0].descriptorCount = MAX_FRAMES_IN_FLIGHT;

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    poolSizes[1].descriptorCount = MAX_FRAMES_IN_FLIGHT;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT * 2;

    r = vkCreateDescriptorPool(device, &poolInfo, nullptr, &descPool);
    VK_CHECKERROR(r);

    VkDescriptorSetAllocateInfo descSetInfo = {};
    descSetInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descSetInfo.descriptorPool = descPool;
    descSetInfo.descriptorSetCount = 1;

    SET_DEBUG_NAME(device, buffersDescSetLayout, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT, "Vertex data Desc set Layout");
    SET_DEBUG_NAME(device, asDescSetLayout, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT, "TLAS Desc set Layout");

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        descSetInfo.pSetLayouts = &buffersDescSetLayout;
        r = vkAllocateDescriptorSets(device, &descSetInfo, &buffersDescSets[i]);
        VK_CHECKERROR(r);

        descSetInfo.pSetLayouts = &asDescSetLayout;
        r = vkAllocateDescriptorSets(device, &descSetInfo, &asDescSets[i]);
        VK_CHECKERROR(r);

        SET_DEBUG_NAME(device, buffersDescSets[i], VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT, "Vertex data Desc set");
        SET_DEBUG_NAME(device, asDescSets[i], VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT, "TLAS Desc set");
    }
}

void ASManager::UpdateBufferDescriptors(uint32_t frameIndex)
{
    const uint32_t bindingCount = 6;

    std::array<VkDescriptorBufferInfo, bindingCount> bufferInfos{};
    std::array<VkWriteDescriptorSet, bindingCount> writes{};

    // buffer infos
    VkDescriptorBufferInfo &stVertsBufInfo = bufferInfos[BINDING_VERTEX_BUFFER_STATIC];
    stVertsBufInfo.buffer = collectorStatic->GetVertexBuffer();
    stVertsBufInfo.offset = 0;
    stVertsBufInfo.range = VK_WHOLE_SIZE;

    VkDescriptorBufferInfo &dnVertsBufInfo = bufferInfos[BINDING_VERTEX_BUFFER_DYNAMIC];
    dnVertsBufInfo.buffer = collectorDynamic[frameIndex]->GetVertexBuffer();
    dnVertsBufInfo.offset = 0;
    dnVertsBufInfo.range = VK_WHOLE_SIZE;

    VkDescriptorBufferInfo &stIndexBufInfo = bufferInfos[BINDING_INDEX_BUFFER_STATIC];
    stIndexBufInfo.buffer = collectorStatic->GetIndexBuffer();
    stIndexBufInfo.offset = 0;
    stIndexBufInfo.range = VK_WHOLE_SIZE;

    VkDescriptorBufferInfo &dnIndexBufInfo = bufferInfos[BINDING_INDEX_BUFFER_DYNAMIC];
    dnIndexBufInfo.buffer = collectorDynamic[frameIndex]->GetIndexBuffer();
    dnIndexBufInfo.offset = 0;
    dnIndexBufInfo.range = VK_WHOLE_SIZE;

    VkDescriptorBufferInfo &gsBufInfo = bufferInfos[BINDING_GEOMETRY_INSTANCES_STATIC];
    gsBufInfo.buffer = collectorStatic->GetGeometryInfosBuffer();
    gsBufInfo.offset = 0;
    gsBufInfo.range = VK_WHOLE_SIZE;

    VkDescriptorBufferInfo &gdBufInfo = bufferInfos[BINDING_GEOMETRY_INSTANCES_DYNAMIC];
    gdBufInfo.buffer = collectorDynamic[frameIndex]->GetGeometryInfosBuffer();
    gdBufInfo.offset = 0;
    gdBufInfo.range = VK_WHOLE_SIZE;


    // writes
    VkWriteDescriptorSet &stVertWrt = writes[BINDING_VERTEX_BUFFER_STATIC];
    stVertWrt.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    stVertWrt.dstSet = buffersDescSets[frameIndex];
    stVertWrt.dstBinding = BINDING_VERTEX_BUFFER_STATIC;
    stVertWrt.dstArrayElement = 0;
    stVertWrt.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    stVertWrt.descriptorCount = 1;
    stVertWrt.pBufferInfo = &stVertsBufInfo;

    VkWriteDescriptorSet &dnVertWrt = writes[BINDING_VERTEX_BUFFER_DYNAMIC];
    dnVertWrt.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    dnVertWrt.dstSet = buffersDescSets[frameIndex];
    dnVertWrt.dstBinding = BINDING_VERTEX_BUFFER_DYNAMIC;
    dnVertWrt.dstArrayElement = 0;
    dnVertWrt.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    dnVertWrt.descriptorCount = 1;
    dnVertWrt.pBufferInfo = &dnVertsBufInfo;

    VkWriteDescriptorSet &stIndexWrt = writes[BINDING_INDEX_BUFFER_STATIC];
    stIndexWrt.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    stIndexWrt.dstSet = buffersDescSets[frameIndex];
    stIndexWrt.dstBinding = BINDING_INDEX_BUFFER_STATIC;
    stIndexWrt.dstArrayElement = 0;
    stIndexWrt.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    stIndexWrt.descriptorCount = 1;
    stIndexWrt.pBufferInfo = &stIndexBufInfo;

    VkWriteDescriptorSet &dnIndexWrt = writes[BINDING_INDEX_BUFFER_DYNAMIC];
    dnIndexWrt.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    dnIndexWrt.dstSet = buffersDescSets[frameIndex];
    dnIndexWrt.dstBinding = BINDING_INDEX_BUFFER_DYNAMIC;
    dnIndexWrt.dstArrayElement = 0;
    dnIndexWrt.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    dnIndexWrt.descriptorCount = 1;
    dnIndexWrt.pBufferInfo = &dnIndexBufInfo;

    VkWriteDescriptorSet &gmStWrt = writes[BINDING_GEOMETRY_INSTANCES_STATIC];
    gmStWrt.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    gmStWrt.dstSet = buffersDescSets[frameIndex];
    gmStWrt.dstBinding = BINDING_GEOMETRY_INSTANCES_STATIC;
    gmStWrt.dstArrayElement = 0;
    gmStWrt.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    gmStWrt.descriptorCount = 1;
    gmStWrt.pBufferInfo = &gsBufInfo;

    VkWriteDescriptorSet &gmDnWrt = writes[BINDING_GEOMETRY_INSTANCES_DYNAMIC];
    gmDnWrt.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    gmDnWrt.dstSet = buffersDescSets[frameIndex];
    gmDnWrt.dstBinding = BINDING_GEOMETRY_INSTANCES_DYNAMIC;
    gmDnWrt.dstArrayElement = 0;
    gmDnWrt.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    gmDnWrt.descriptorCount = 1;
    gmDnWrt.pBufferInfo = &gdBufInfo;

    vkUpdateDescriptorSets(device, writes.size(), writes.data(), 0, nullptr);
}

void ASManager::UpdateASDescriptors(uint32_t frameIndex)
{
    VkWriteDescriptorSetAccelerationStructureKHR asWrt = {};
    asWrt.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
    asWrt.accelerationStructureCount = 1;
    asWrt.pAccelerationStructures = &tlas[frameIndex].as;

    VkWriteDescriptorSet wrt = {};
    wrt.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    wrt.pNext = &asWrt;
    wrt.dstSet = asDescSets[frameIndex];
    wrt.dstBinding = BINDING_ACCELERATION_STRUCTURE;
    wrt.dstArrayElement = 0;
    wrt.descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    wrt.descriptorCount = 1;

    vkUpdateDescriptorSets(device, 1, &wrt, 0, nullptr);
}

#pragma endregion 

ASManager::~ASManager()
{
    for (auto &as : allStaticBlas)
    {
        DestroyAS(as);
    }

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        for (auto &as : allDynamicBlas[i])
        {
            DestroyAS(as);
        }

        DestroyAS(tlas[i]);
    }

    vkDestroyDescriptorPool(device, descPool, nullptr);
    vkDestroyDescriptorSetLayout(device, buffersDescSetLayout, nullptr);
    vkDestroyDescriptorSetLayout(device, asDescSetLayout, nullptr);
    vkDestroyFence(device, staticCopyFence, nullptr);
}

void ASManager::SetupBLAS(AccelerationStructure &as,
                          const std::shared_ptr<VertexCollector> &vertCollector)
{
    auto filter = as.filter;
    const std::vector<VkAccelerationStructureGeometryKHR> &geoms = vertCollector->GetASGeometries(filter);

    if (geoms.empty())
    {
        return;
    }

    const std::vector<VkAccelerationStructureBuildRangeInfoKHR> &ranges = vertCollector->GetASBuildRangeInfos(filter);
    const std::vector<uint32_t> &primCounts = vertCollector->GetPrimitiveCounts(filter);

    const bool fastTrace = !IsFastBuild(filter);
    const bool update = false;

    // get AS size and create buffer for AS
    const auto buildSizes = asBuilder->GetBottomBuildSizes(
        geoms.size(), geoms.data(), primCounts.data(), fastTrace);

    // if no buffer, or it was created, but its size is too small for current AS
    if (!as.buffer.IsInitted() || as.buffer.GetSize() < buildSizes.accelerationStructureSize)
    {
        // destroy
        DestroyAS(as);

        // create
        CreateASBuffer(as, buildSizes.accelerationStructureSize, "BLAS buffer");

        VkAccelerationStructureCreateInfoKHR blasInfo = {};
        blasInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
        blasInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
        blasInfo.size = buildSizes.accelerationStructureSize;
        blasInfo.buffer = as.buffer.GetBuffer();
        VkResult r = svkCreateAccelerationStructureKHR(device, &blasInfo, nullptr, &as.as);
        VK_CHECKERROR(r);

        const char *debugName = GetVertexCollectorFilterTypeFlagsNameForBLAS(filter);
        SET_DEBUG_NAME(device, as.as, VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR_EXT, debugName);
    }

    // add BLAS, all passed arrays must be alive until BuildBottomLevel() call
    asBuilder->AddBLAS(as.as, geoms.size(),
                       geoms.data(), ranges.data(),
                       buildSizes,
                       fastTrace, update);
}

void ASManager::UpdateBLAS(AccelerationStructure &as, 
                                 const std::shared_ptr<VertexCollector> &vertCollector)
{
    auto filter = as.filter;
    const std::vector<VkAccelerationStructureGeometryKHR> &geoms = vertCollector->GetASGeometries(filter);

    if (geoms.empty())
    {
        return;
    }

    const std::vector<VkAccelerationStructureBuildRangeInfoKHR> &ranges = vertCollector->GetASBuildRangeInfos(filter);
    const std::vector<uint32_t> &primCounts = vertCollector->GetPrimitiveCounts(filter);

    const bool fastTrace = !IsFastBuild(filter);

    // must be just updated
    const bool update = true;

    const auto buildSizes = asBuilder->GetBottomBuildSizes(
        geoms.size(), geoms.data(), primCounts.data(), fastTrace);

    assert(as.buffer.IsInitted() && as.buffer.GetSize() >= buildSizes.accelerationStructureSize);

    // add BLAS, all passed arrays must be alive until BuildBottomLevel() call
    asBuilder->AddBLAS(as.as, geoms.size(),
                       geoms.data(), ranges.data(),
                       buildSizes,
                       fastTrace, update);
}

// separate functions to make adding between Begin..Geometry() and Submit..Geometry() a bit clearer

uint32_t ASManager::AddStaticGeometry(const RgGeometryUploadInfo &info)
{
    if (info.geomType == RG_GEOMETRY_TYPE_STATIC || info.geomType == RG_GEOMETRY_TYPE_STATIC_MOVABLE)
    {
        MaterialTextures materials[3] =
        {
            textureMgr->GetMaterialTextures(info.geomMaterial.layerMaterials[0]),
            textureMgr->GetMaterialTextures(info.geomMaterial.layerMaterials[1]),
            textureMgr->GetMaterialTextures(info.geomMaterial.layerMaterials[2])
        };

        return collectorStatic->AddGeometry(info, materials);
    }

    assert(0);
    return UINT32_MAX;
}

uint32_t ASManager::AddDynamicGeometry(const RgGeometryUploadInfo &info, uint32_t frameIndex)
{
    if (info.geomType == RG_GEOMETRY_TYPE_DYNAMIC)
    {
        MaterialTextures materials[3] =
        {
            textureMgr->GetMaterialTextures(info.geomMaterial.layerMaterials[0]),
            textureMgr->GetMaterialTextures(info.geomMaterial.layerMaterials[1]),
            textureMgr->GetMaterialTextures(info.geomMaterial.layerMaterials[2])
        };

        return collectorDynamic[frameIndex]->AddGeometry(info, materials);
    }

    assert(0);
    return UINT32_MAX;
}

void ASManager::ResetStaticGeometry()
{
    collectorStatic->Reset();
}

void ASManager::BeginStaticGeometry()
{
    // the whole static vertex data must be recreated, clear previous data
    collectorStatic->Reset();
    collectorStatic->BeginCollecting();
}

void ASManager::SubmitStaticGeometry()
{
    collectorStatic->EndCollecting();

    typedef VertexCollectorFilterTypeFlagBits FT;

    auto staticFlags = FT::CF_STATIC_NON_MOVABLE | FT::CF_STATIC_MOVABLE;

    // destroy previous static
    for (auto &staticBlas : allStaticBlas)
    {
        assert(!(staticBlas.filter & FT::CF_DYNAMIC));

        // if flags have any of static bits
        if (staticBlas.filter & staticFlags)
        {
            DestroyAS(staticBlas);
        }
    }

    assert(asBuilder->IsEmpty());

    // skip if all static geometries are empty
    if (collectorStatic->AreGeometriesEmpty(staticFlags))
    {
        return;
    }

    VkCommandBuffer cmd = cmdManager->StartGraphicsCmd();

    // copy from staging with barrier
    collectorStatic->CopyFromStaging(cmd, true);

    // setup static blas
    for (auto &staticBlas : allStaticBlas)
    {
        // if flags have any of static bits
        if (staticBlas.filter & staticFlags)
        {
            SetupBLAS(staticBlas, collectorStatic);
        }
    }
    
    // build AS
    asBuilder->BuildBottomLevel(cmd);

    SET_CHECKPOINT(cmd, RG_CHECKPOINT_BUILD_STATIC_BLAS);

    // submit and wait
    cmdManager->Submit(cmd, staticCopyFence);
    Utils::WaitAndResetFence(device, staticCopyFence);
}

void ASManager::BeginDynamicGeometry(uint32_t frameIndex)
{
    // dynamic AS must be recreated
    collectorDynamic[frameIndex]->Reset();
    collectorDynamic[frameIndex]->BeginCollecting();
}

void ASManager::SubmitDynamicGeometry(VkCommandBuffer cmd, uint32_t frameIndex)
{
    typedef VertexCollectorFilterTypeFlagBits FT;

    const auto &colDyn = collectorDynamic[frameIndex];

    colDyn->EndCollecting();
    colDyn->CopyFromStaging(cmd, false);

    assert(asBuilder->IsEmpty());

    if (colDyn->AreGeometriesEmpty(FT::CF_DYNAMIC))
    {
        return;
    }

    // recreate dynamic blas
    for (auto &dynamicBlas : allDynamicBlas[frameIndex])
    {
        // must be dynamic
        assert(dynamicBlas.filter & FT::CF_DYNAMIC);

        SetupBLAS(dynamicBlas, colDyn);
    }

    // build BLAS
    asBuilder->BuildBottomLevel(cmd);

    SET_CHECKPOINT(cmd, RG_CHECKPOINT_BUILD_DYNAMIC_BLAS);
}

void ASManager::UpdateStaticMovableTransform(uint32_t geomIndex, const RgTransform &transform)
{
    collectorStatic->UpdateTransform(geomIndex, transform);
}

void ASManager::ResubmitStaticMovable(VkCommandBuffer cmd)
{
    typedef VertexCollectorFilterTypeFlagBits FT;

    if (collectorStatic->AreGeometriesEmpty(FT::CF_STATIC_MOVABLE))
    {
        return;
    }

    assert(asBuilder->IsEmpty());

    // update movable blas
    for (auto &blas : allStaticBlas)
    {
        assert(!(blas.filter & FT::CF_DYNAMIC));

        // if flags have any of static bits
        if (blas.filter & FT::CF_STATIC_MOVABLE)
        {
            auto &movableBlas = blas;

            UpdateBLAS(blas, collectorStatic);
        }
    }

    asBuilder->BuildBottomLevel(cmd);
    Utils::ASBuildMemoryBarrier(cmd);

    SET_CHECKPOINT(cmd, RG_CHECKPOINT_BUILD_STATIC_BLAS_UPDATE);
}

bool ASManager::SetupTLASInstance(const AccelerationStructure &as, VkAccelerationStructureInstanceKHR &instance)
{
    if (as.as == VK_NULL_HANDLE)
    {
        return false;
    }

    typedef VertexCollectorFilterTypeFlagBits FT;

    auto filter = as.filter;

    instance.accelerationStructureReference = GetASAddress(as);

    instance.transform = 
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f
    };

    if (filter & FT::CF_DYNAMIC)
    {
        // for choosing buffers with dynamic data
        instance.instanceCustomIndex = INSTANCE_CUSTOM_INDEX_FLAG_DYNAMIC;
    }
    else
    {
        instance.instanceCustomIndex = 0;
    }

    // blended geometries don't have shadows
    if (filter & (FT::PT_BLEND_ADDITIVE | FT::PT_BLEND_UNDER))
    {
        instance.mask = ~INSTANCE_MASK_HAS_SHADOWS;
    }
    else
    {
        instance.mask = INSTANCE_MASK_HAS_SHADOWS;
    }

    if (filter & FT::PT_OPAQUE)
    {
        instance.instanceShaderBindingTableRecordOffset = SBT_INDEX_HITGROUP_FULLY_OPAQUE;
        instance.flags =
            VK_GEOMETRY_INSTANCE_FORCE_OPAQUE_BIT_KHR |
            VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
    }
    else 
    {
        if (filter & FT::PT_ALPHA_TESTED)
        {
            instance.instanceShaderBindingTableRecordOffset = SBT_INDEX_HITGROUP_ALPHA_TESTED;
        }
        else if (filter &FT::PT_BLEND_ADDITIVE)
        {
            instance.instanceShaderBindingTableRecordOffset = SBT_INDEX_HITGROUP_BLEND_ADDITIVE;
        }
        else if (filter & FT::PT_BLEND_UNDER)
        {
            instance.instanceShaderBindingTableRecordOffset = SBT_INDEX_HITGROUP_BLEND_UNDER;
        }
        
        instance.flags =
            VK_GEOMETRY_INSTANCE_FORCE_NO_OPAQUE_BIT_KHR |
            VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
    }

    return true;
}

bool ASManager::TryBuildTLAS(VkCommandBuffer cmd, uint32_t frameIndex, const std::shared_ptr<GlobalUniform> &uniform)
{
    typedef VertexCollectorFilterTypeFlagBits FT;

    // BLAS instances
    uint32_t instanceCount = 0;
    VkAccelerationStructureInstanceKHR instances[32] = {};

    // for getting offsets in geomInfos buffer by instance ID in shaders,
    // this array will be copied to uniform buffer
    // note: std140 requires elements to be aligned by sizeof(vec4)
    int32_t instanceGeomInfoOffset[MAX_TOP_LEVEL_INSTANCE_COUNT * 4];


    for (auto &blas : allStaticBlas)
    {
        assert(!(blas.filter & FT::CF_DYNAMIC));

        bool isAdded = SetupTLASInstance(blas, instances[instanceCount]);

        if (isAdded)
        {
            instanceGeomInfoOffset[instanceCount * 4] = VertexCollectorFilterTypeFlagsToOffset(blas.filter) * MAX_BOTTOM_LEVEL_GEOMETRIES_COUNT;

            instanceCount++;
            assert(instanceCount < MAX_TOP_LEVEL_INSTANCE_COUNT);
        }
    }

    for (auto &blas : allDynamicBlas[frameIndex])
    {
        assert(blas.filter & FT::CF_DYNAMIC);

        bool isAdded = SetupTLASInstance(blas, instances[instanceCount]);

        if (isAdded)
        {
            instanceGeomInfoOffset[instanceCount * 4] = VertexCollectorFilterTypeFlagsToOffset(blas.filter) * MAX_BOTTOM_LEVEL_GEOMETRIES_COUNT;

            instanceCount++;
            assert(instanceCount < MAX_TOP_LEVEL_INSTANCE_COUNT);
        }
    }

    if (instanceCount == 0)
    {
        return false;
    }

    // copy geometry offsets to uniform to access geomInfos
    // with instance ID and geometry index in shaders
    memcpy(uniform->GetData()->instanceGeomInfoOffset, instanceGeomInfoOffset, instanceCount * 4 * sizeof(int32_t));


    // fill buffer
    void *mapped = instanceBuffers[frameIndex].Map();
    memcpy(mapped, instances, instanceCount * sizeof(VkAccelerationStructureInstanceKHR));

    instanceBuffers[frameIndex].Unmap();

    VkAccelerationStructureGeometryKHR instGeom = {};
    instGeom.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
    instGeom.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
    instGeom.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
    auto &instData = instGeom.geometry.instances;
    instData.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    instData.arrayOfPointers = VK_FALSE;
    instData.data.deviceAddress = instanceBuffers[frameIndex].GetAddress();

    auto &curTlas = tlas[frameIndex];

    // get AS size and create buffer for AS
    const auto buildSizes = asBuilder->GetTopBuildSizes(&instGeom, &instanceCount, false);

    // if previous buffer's size is not enough
    if (curTlas.buffer.GetSize() < buildSizes.accelerationStructureSize)
    {
        // destroy
        DestroyAS(curTlas);

        // create
        CreateASBuffer(curTlas, buildSizes.accelerationStructureSize, "TLAS buffer");

        VkAccelerationStructureCreateInfoKHR tlasInfo = {};
        tlasInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
        tlasInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
        tlasInfo.size = buildSizes.accelerationStructureSize;
        tlasInfo.buffer = curTlas.buffer.GetBuffer();
        VkResult r = svkCreateAccelerationStructureKHR(device, &tlasInfo, nullptr, &curTlas.as);
        VK_CHECKERROR(r);

        SET_DEBUG_NAME(device, curTlas.as, VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR_EXT, "TLAS");
    }

    assert(asBuilder->IsEmpty());

    VkAccelerationStructureBuildRangeInfoKHR range = {};
    range.primitiveCount = instanceCount;

    asBuilder->AddTLAS(curTlas.as, &instGeom, &range, buildSizes, true, false);
    asBuilder->BuildTopLevel(cmd);

    Utils::ASBuildMemoryBarrier(cmd);

    UpdateASDescriptors(frameIndex);

    return true;
}

void ASManager::CreateASBuffer(AccelerationStructure &as, VkDeviceSize size, const char *debugName)
{
    as.buffer.Init(
        allocator, size,
        VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        debugName
    );
}

void ASManager::DestroyAS(AccelerationStructure &as)
{
    as.buffer.Destroy();

    if (as.as != VK_NULL_HANDLE)
    {
        svkDestroyAccelerationStructureKHR(device, as.as, nullptr);
        as.as = VK_NULL_HANDLE;
    }
}

VkDeviceAddress ASManager::GetASAddress(const AccelerationStructure& as)
{
    return GetASAddress(as.as);
}

VkDeviceAddress ASManager::GetASAddress(VkAccelerationStructureKHR as)
{
    VkAccelerationStructureDeviceAddressInfoKHR addressInfo = {};
    addressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
    addressInfo.accelerationStructure = as;

    return svkGetAccelerationStructureDeviceAddressKHR(device, &addressInfo);
}

bool ASManager::IsFastBuild(VertexCollectorFilterTypeFlags filter)
{
    typedef VertexCollectorFilterTypeFlagBits FT;

    // fast trace for static
    // fast build for dynamic
    return filter & FT::CF_DYNAMIC;
}

VkDescriptorSet ASManager::GetBuffersDescSet(uint32_t frameIndex) const
{
    return buffersDescSets[frameIndex];
}

VkDescriptorSet ASManager::GetTLASDescSet(uint32_t frameIndex) const
{
    // if TLAS wasn't built, return null
    if (tlas[frameIndex].as == VK_NULL_HANDLE)
    {
        return VK_NULL_HANDLE;
    }

    return asDescSets[frameIndex];
}

VkDescriptorSetLayout ASManager::GetBuffersDescSetLayout() const
{
    return buffersDescSetLayout;
}

VkDescriptorSetLayout ASManager::GetTLASDescSetLayout() const
{
    return asDescSetLayout;
}
