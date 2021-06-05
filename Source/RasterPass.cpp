// Copyright (c) 2021 Sultim Tsyrendashiev
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

#include "RasterPass.h"

#include "Generated/ShaderCommonCFramebuf.h"
#include "Rasterizer.h"
#include "RgException.h"
#include "Utils.h"


constexpr const char *VERT_SHADER = "VertRasterizer";
constexpr const char *FRAG_SHADER = "FragRasterizer";
constexpr VkFormat COLOR_FORMAT = VK_FORMAT_R8G8B8A8_UNORM;
constexpr VkFormat DEPTH_FORMAT = VK_FORMAT_X8_D24_UNORM_PACK32;
constexpr const char *DEPTH_FORMAT_NAME = "VK_FORMAT_X8_D24_UNORM_PACK32";


RTGL1::RasterPass::RasterPass(
    VkDevice _device, 
    VkPhysicalDevice _physDevice,
    VkPipelineLayout _pipelineLayout,
    const std::shared_ptr<ShaderManager> &_shaderManager,
    const std::shared_ptr<Framebuffers> &_storageFramebuffers,
    const RgInstanceCreateInfo &_instanceInfo)
:
    device(_device),
    rasterRenderPass(VK_NULL_HANDLE),
    rasterSkyRenderPass(VK_NULL_HANDLE),
    rasterWidth(0),
    rasterHeight(0),
    rasterFramebuffers{},
    rasterSkyFramebuffers{},
    colorAttchs{},
    depthAttchs{}
{
    VkFormatProperties props = {};
    vkGetPhysicalDeviceFormatProperties(_physDevice, DEPTH_FORMAT, &props);
    if ((props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == 0)
    {
        using namespace std::string_literals;
        throw RgException(RG_GRAPHICS_API_ERROR, "Depth format is not supported: "s + DEPTH_FORMAT_NAME);
    }

    CreateRasterRenderPass(COLOR_FORMAT,
                           ShFramebuffers_Formats[FB_IMAGE_INDEX_ALBEDO],
                           DEPTH_FORMAT);

    rasterPipelines = std::make_shared<RasterizerPipelines>(device, _pipelineLayout, rasterRenderPass, _instanceInfo.rasterizedVertexColorGamma);
    rasterPipelines->SetShaders(_shaderManager.get(), VERT_SHADER, FRAG_SHADER);

    rasterSkyPipelines= std::make_shared<RasterizerPipelines>(device, _pipelineLayout, rasterSkyRenderPass, _instanceInfo.rasterizedVertexColorGamma);
    rasterSkyPipelines->SetShaders(_shaderManager.get(), VERT_SHADER, FRAG_SHADER);

    depthCopying = std::make_shared<DepthCopying>(device, DEPTH_FORMAT, _shaderManager, _storageFramebuffers);
}

RTGL1::RasterPass::~RasterPass()
{
    vkDestroyRenderPass(device, rasterRenderPass, nullptr);
    vkDestroyRenderPass(device, rasterSkyRenderPass, nullptr);
    DestroyFramebuffers();
}

void RTGL1::RasterPass::PrepareForFinal(
    VkCommandBuffer cmd, uint32_t frameIndex,
    const std::shared_ptr<Framebuffers> &storageFramebuffers, 
    bool werePrimaryTraced)
{
    assert(rasterWidth > 0 && rasterHeight > 0);


    // firstly, copy data from storage buffer to depth buffer,
    // and only after getting correct depth buffer, draw the geometry
    // if no primary rays were traced, just clear depth buffer without copying
    depthCopying->Process(cmd, frameIndex, storageFramebuffers, rasterWidth, rasterHeight, !werePrimaryTraced);


    // also, copy color attachment data from the FINAL framebuf
    VkImageBlit region = {};

    region.srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
    region.srcOffsets[0] = { 0, 0, 0 };
    region.srcOffsets[1] = { static_cast<int32_t>(rasterWidth), static_cast<int32_t>(rasterHeight), 1 };

    region.dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
    region.dstOffsets[0] = { 0, 0, 0 };
    region.dstOffsets[1] = { static_cast<int32_t>(rasterWidth), static_cast<int32_t>(rasterHeight), 1 };

    vkCmdBlitImage(cmd, 
                   storageFramebuffers->GetImage(FB_IMAGE_INDEX_FINAL, frameIndex), VK_IMAGE_LAYOUT_GENERAL,
                   colorAttchs.image[frameIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                   1, &region, VK_FILTER_NEAREST);
}

void RTGL1::RasterPass::CreateFramebuffers(uint32_t renderWidth, uint32_t renderHeight,
                                           const std::shared_ptr<Framebuffers> &storageFramebuffers,
                                           const std::shared_ptr<MemoryAllocator> &allocator,
                                           const std::shared_ptr<CommandBufferManager> &cmdManager)
{
    CreateImages(renderWidth, renderHeight, allocator, cmdManager);

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        assert(rasterFramebuffers[i] == VK_NULL_HANDLE);
        assert(rasterSkyFramebuffers[i] == VK_NULL_HANDLE);

        VkFramebufferCreateInfo fbInfo = {};
        fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbInfo.width = renderWidth;
        fbInfo.height = renderHeight;
        fbInfo.layers = 1;

        VkImageView attchs[] =
        {
            VK_NULL_HANDLE,
            depthAttchs.view[i]
        };

        fbInfo.attachmentCount = sizeof(attchs) / sizeof(attchs[0]);
        fbInfo.pAttachments = attchs;

        {
            fbInfo.renderPass = rasterRenderPass;

            attchs[0] = colorAttchs.view[i]; 

            VkResult r = vkCreateFramebuffer(device, &fbInfo, nullptr, &rasterFramebuffers[i]);
            VK_CHECKERROR(r);

            SET_DEBUG_NAME(device, rasterFramebuffers[i], VK_OBJECT_TYPE_FRAMEBUFFER, "Rasterizer raster framebuffer");
        }

        {
            fbInfo.renderPass = rasterSkyRenderPass;

            attchs[0] = storageFramebuffers->GetImageView(FB_IMAGE_INDEX_ALBEDO, i);

            VkResult r = vkCreateFramebuffer(device, &fbInfo, nullptr, &rasterSkyFramebuffers[i]);
            VK_CHECKERROR(r);

            SET_DEBUG_NAME(device, rasterSkyFramebuffers[i], VK_OBJECT_TYPE_FRAMEBUFFER, "Rasterizer raster sky framebuffer");
        }
    }

    depthCopying->CreateFramebuffers(depthAttchs.view, renderWidth, renderHeight);

    this->rasterWidth = renderWidth;
    this->rasterHeight = renderHeight;
}

void RTGL1::RasterPass::DestroyFramebuffers()
{
    depthCopying->DestroyFramebuffers();

    DestroyImages();

    for (VkFramebuffer &fb : rasterFramebuffers)
    {
        if (fb != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(device, fb, nullptr);
            fb = VK_NULL_HANDLE;
        }
    }

    for (VkFramebuffer &fb : rasterSkyFramebuffers)
    {
        if (fb != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(device, fb, nullptr);
            fb = VK_NULL_HANDLE;
        }
    }
}

VkRenderPass RTGL1::RasterPass::GetRasterRenderPass() const
{
    return rasterRenderPass;
}

VkRenderPass RTGL1::RasterPass::GetSkyRasterRenderPass() const
{
    return rasterSkyRenderPass;
}

const std::shared_ptr<RTGL1::RasterizerPipelines> &RTGL1::RasterPass::GetRasterPipelines() const
{
    return rasterPipelines;
}

const std::shared_ptr<RTGL1::RasterizerPipelines> &RTGL1::RasterPass::GetSkyRasterPipelines() const
{
    return rasterSkyPipelines;
}

uint32_t RTGL1::RasterPass::GetRasterWidth() const
{
    return rasterWidth;
}

uint32_t RTGL1::RasterPass::GetRasterHeight() const
{
    return rasterHeight;
}

VkFramebuffer RTGL1::RasterPass::GetFramebuffer(uint32_t frameIndex) const
{
    return rasterFramebuffers[frameIndex];
}

VkFramebuffer RTGL1::RasterPass::GetSkyFramebuffer(uint32_t frameIndex) const
{
    return rasterSkyFramebuffers[frameIndex];
}

VkImage RTGL1::RasterPass::GetRasterAttachmentImage(uint32_t frameIndex) const
{
    return colorAttchs.image[frameIndex];
}

VkImageLayout RTGL1::RasterPass::GetRasterAttachmentImageLayout() const
{
    return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
}

void RTGL1::RasterPass::OnShaderReload(const ShaderManager *shaderManager)
{
    rasterPipelines->Clear();
    rasterSkyPipelines->Clear();

    rasterPipelines->SetShaders(shaderManager, VERT_SHADER, FRAG_SHADER);
    rasterSkyPipelines->SetShaders(shaderManager, VERT_SHADER, FRAG_SHADER);

    depthCopying->OnShaderReload(shaderManager);
}

void RTGL1::RasterPass::CreateRasterRenderPass(VkFormat rasterAttchFormat, VkFormat skyAttchFormat, VkFormat depthAttchFormat)
{
    const int attchCount = 2;
    VkAttachmentDescription attchs[attchCount] = {};

    auto &colorAttch = attchs[0];
    colorAttch.format = VK_FORMAT_UNDEFINED;
    colorAttch.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttch.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    colorAttch.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttch.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttch.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // dst optimal for blitting from final frambebuf
    colorAttch.initialLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    colorAttch.finalLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    auto &depthAttch = attchs[1];
    depthAttch.format = depthAttchFormat;
    depthAttch.samples = VK_SAMPLE_COUNT_1_BIT;
    // will be overwritten
    depthAttch.loadOp = VK_ATTACHMENT_LOAD_OP_MAX_ENUM;
    depthAttch.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttch.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttch.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // depth image was already transitioned
    // by depthCopying for rasterRenderPass
    // and manually for rasterSkyRenderPass
    depthAttch.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    depthAttch.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


    VkAttachmentReference colorRef = {};
    colorRef.attachment = 0;
    colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthRef = {};
    depthRef.attachment = 1;
    depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorRef;
    subpass.pDepthStencilAttachment = &depthRef;


    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                               VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


    VkRenderPassCreateInfo passInfo = {};
    passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    passInfo.attachmentCount = attchCount;
    passInfo.pAttachments = attchs;
    passInfo.subpassCount = 1;
    passInfo.pSubpasses = &subpass;
    passInfo.dependencyCount = 1;
    passInfo.pDependencies = &dependency;

    {
        colorAttch.format = rasterAttchFormat;
        // load depth data from depthCopying
        depthAttch.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;

        VkResult r = vkCreateRenderPass(device, &passInfo, nullptr, &rasterRenderPass);
        VK_CHECKERROR(r);

        SET_DEBUG_NAME(device, rasterRenderPass, VK_OBJECT_TYPE_RENDER_PASS, "Rasterizer raster render pass");
    }

    {
        colorAttch.format = skyAttchFormat;
        depthAttch.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

        VkResult r = vkCreateRenderPass(device, &passInfo, nullptr, &rasterSkyRenderPass);
        VK_CHECKERROR(r);

        SET_DEBUG_NAME(device, rasterSkyRenderPass, VK_OBJECT_TYPE_RENDER_PASS, "Rasterizer raster sky render pass");
    }
}

void RTGL1::RasterPass::CreateImages(uint32_t width, uint32_t height,
                                           const std::shared_ptr<MemoryAllocator> &allocator, 
                                           const std::shared_ptr<CommandBufferManager> &cmdManager)
{
    VkResult r;

    VkCommandBuffer cmd = cmdManager->StartGraphicsCmd();

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        assert(depthAttchs.image[i] == VK_NULL_HANDLE);
        assert(depthAttchs.view[i] == VK_NULL_HANDLE);
        assert(depthAttchs.memory[i] == VK_NULL_HANDLE);

        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.flags = 0;
        imageInfo.extent = { width, height, 1 };
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        {
            imageInfo.format = DEPTH_FORMAT;
            imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

            r = vkCreateImage(device, &imageInfo, nullptr, &depthAttchs.image[i]);
            VK_CHECKERROR(r);
            SET_DEBUG_NAME(device, depthAttchs.image[i], VK_OBJECT_TYPE_IMAGE, "Rasterizer raster pass depth image");
        }
        {
            imageInfo.format = COLOR_FORMAT;
            // dst - for copying from final framebuf,
            // src - for copying to swapchain
            imageInfo.usage = 
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | 
                VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

            r = vkCreateImage(device, &imageInfo, nullptr, &colorAttchs.image[i]);
            VK_CHECKERROR(r);
            SET_DEBUG_NAME(device, colorAttchs.image[i], VK_OBJECT_TYPE_IMAGE, "Rasterizer raster pass color image");
        }


        // allocate dedicated memory
        {
            VkMemoryRequirements memReqs;
            vkGetImageMemoryRequirements(device, depthAttchs.image[i], &memReqs);

            depthAttchs.memory[i] = allocator->AllocDedicated(memReqs, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            SET_DEBUG_NAME(device, depthAttchs.memory[i], VK_OBJECT_TYPE_DEVICE_MEMORY, "Rasterizer raster pass depth memory");

            if (depthAttchs.memory[i] == VK_NULL_HANDLE)
            {
                vkDestroyImage(device, depthAttchs.image[i], nullptr);
                depthAttchs.image[i] = VK_NULL_HANDLE;

                throw RgException(RG_GRAPHICS_API_ERROR, "Can't allocate device memory for raster pass depth attachment");
            }

            r = vkBindImageMemory(device, depthAttchs.image[i], depthAttchs.memory[i], 0);
            VK_CHECKERROR(r);
        }
        {
            VkMemoryRequirements memReqs;
            vkGetImageMemoryRequirements(device, colorAttchs.image[i], &memReqs);

            colorAttchs.memory[i] = allocator->AllocDedicated(memReqs, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            SET_DEBUG_NAME(device, colorAttchs.memory[i], VK_OBJECT_TYPE_DEVICE_MEMORY, "Rasterizer raster pass color memory");

            if (colorAttchs.memory[i] == VK_NULL_HANDLE)
            {
                vkDestroyImage(device, colorAttchs.image[i], nullptr);
                colorAttchs.image[i] = VK_NULL_HANDLE;

                throw RgException(RG_GRAPHICS_API_ERROR, "Can't allocate device memory for raster pass color attachment");
            }

            r = vkBindImageMemory(device, colorAttchs.image[i], colorAttchs.memory[i], 0);
            VK_CHECKERROR(r);
        }


        // create image view
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.subresourceRange = {};
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        {
            viewInfo.format = DEPTH_FORMAT;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            viewInfo.image = depthAttchs.image[i];

            r = vkCreateImageView(device, &viewInfo, nullptr, &depthAttchs.view[i]);
            VK_CHECKERROR(r);
            SET_DEBUG_NAME(device, depthAttchs.view[i], VK_OBJECT_TYPE_IMAGE_VIEW, "Rasterizer raster pass depth image view");
        }
        {
            viewInfo.format = COLOR_FORMAT;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.image = colorAttchs.image[i];

            r = vkCreateImageView(device, &viewInfo, nullptr, &colorAttchs.view[i]);
            VK_CHECKERROR(r);
            SET_DEBUG_NAME(device, colorAttchs.view[i], VK_OBJECT_TYPE_IMAGE_VIEW, "Rasterizer raster pass color image view");
        }


        // make transition from undefined manually,
        // so depthAttch.initialLayout can be specified as DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        VkImageMemoryBarrier imageBarriers[2] = {};

        {
            VkImageMemoryBarrier &br = imageBarriers[0];
            br.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            br.image = depthAttchs.image[i];
            br.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            br.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            br.srcAccessMask = 0;
            br.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
            br.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            br.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            br.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            br.subresourceRange.baseMipLevel = 0;
            br.subresourceRange.levelCount = 1;
            br.subresourceRange.baseArrayLayer = 0;
            br.subresourceRange.layerCount = 1;
        }
        {
            VkImageMemoryBarrier &br = imageBarriers[1];
            br.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            br.image = colorAttchs.image[i];
            br.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            br.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            br.srcAccessMask = 0;
            br.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            br.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            br.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            br.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            br.subresourceRange.baseMipLevel = 0;
            br.subresourceRange.levelCount = 1;
            br.subresourceRange.baseArrayLayer = 0;
            br.subresourceRange.layerCount = 1;
        }

        vkCmdPipelineBarrier(
            cmd,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0,
            0, nullptr,
            0, nullptr,
            sizeof(imageBarriers) / sizeof(imageBarriers[0]), imageBarriers);
    }

    cmdManager->Submit(cmd);
    cmdManager->WaitGraphicsIdle();
}

void RTGL1::RasterPass::DestroyImages()
{
    DestroyPassImageDef(device, colorAttchs);
    DestroyPassImageDef(device, depthAttchs);
}

void RTGL1::RasterPass::DestroyPassImageDef(VkDevice device, PassImageDef &def)
{
    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        assert(( def.image[i] &&  def.view[i] &&  def.memory[i]) ||
               (!def.image[i] && !def.view[i] && !def.memory[i]));
        
        if (def.image[i] != VK_NULL_HANDLE)
        {
            vkDestroyImage(device, def.image[i], nullptr);
            vkDestroyImageView(device, def.view[i], nullptr);
            vkFreeMemory(device, def.memory[i], nullptr);

            def.image[i] = VK_NULL_HANDLE;
            def.view[i] = VK_NULL_HANDLE;
            def.memory[i] = VK_NULL_HANDLE;
        }
    }
}
