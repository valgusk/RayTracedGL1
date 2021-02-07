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

#pragma once

#include <cassert>
#include <memory>
#include <vulkan/vulkan.h>

#define MAX_FRAMES_IN_FLIGHT 2

#pragma region extension functions

// extension functions' lists
#define VK_INSTANCE_DEBUG_UTILS_FUNCTION_LIST \
	VK_EXTENSION_FUNCTION(vkCmdBeginDebugUtilsLabelEXT) \
	VK_EXTENSION_FUNCTION(vkCmdEndDebugUtilsLabelEXT) \
	VK_EXTENSION_FUNCTION(vkCreateDebugUtilsMessengerEXT) \
	VK_EXTENSION_FUNCTION(vkDestroyDebugUtilsMessengerEXT)

#define VK_DEVICE_FUNCTION_LIST \
	VK_EXTENSION_FUNCTION(vkCreateAccelerationStructureKHR) \
	VK_EXTENSION_FUNCTION(vkDestroyAccelerationStructureKHR) \
	VK_EXTENSION_FUNCTION(vkGetRayTracingShaderGroupHandlesKHR) \
	VK_EXTENSION_FUNCTION(vkCreateRayTracingPipelinesKHR) \
	VK_EXTENSION_FUNCTION(vkGetAccelerationStructureDeviceAddressKHR) \
	VK_EXTENSION_FUNCTION(vkGetAccelerationStructureBuildSizesKHR) \
	VK_EXTENSION_FUNCTION(vkCmdBuildAccelerationStructuresKHR) \
	VK_EXTENSION_FUNCTION(vkGetQueueCheckpointDataNV) \
	VK_EXTENSION_FUNCTION(vkCmdSetCheckpointNV) \
	VK_EXTENSION_FUNCTION(vkCmdTraceRaysKHR)

#define VK_DEVICE_DEBUG_UTILS_FUNCTION_LIST \
	VK_EXTENSION_FUNCTION(vkDebugMarkerSetObjectNameEXT)


// extension functions' declarations
#define VK_EXTENSION_FUNCTION(fname) extern PFN_##fname s##fname;
    VK_INSTANCE_DEBUG_UTILS_FUNCTION_LIST
    VK_DEVICE_FUNCTION_LIST
    VK_DEVICE_DEBUG_UTILS_FUNCTION_LIST
#undef VK_EXTENSION_FUNCTION

void InitInstanceExtensionFunctions_DebugUtils(VkInstance instance);
void InitDeviceExtensionFunctions(VkDevice device);
void InitDeviceExtensionFunctions_DebugUtils(VkDevice device);

#pragma endregion

enum RgDebugCheckpoints
{
    RG_CHECKPOINT_BEGIN_FRAME,
    RG_CHECKPOINT_BUILD_STATIC_BLAS,
    RG_CHECKPOINT_BUILD_STATIC_BLAS_UPDATE,
    RG_CHECKPOINT_BUILD_DYNAMIC_BLAS,
    RG_CHECKPOINT_BUILD_TLAS,
    RG_CHECKPOINT_TEXTURE_UPLOAD,
    RG_CHECKPOINT_TEXTURE_COPY_STAGING_TO_IMAGE,
    RG_CHECKPOINT_TEXTURE_PREPARE_MIPMAPS,
    RG_CHECKPOINT_TRACE_BIND_DESC_SETS,
    RG_CHECKPOINT_TRACE_PRIMARY,
    RG_CHECKPOINT_TRACE_DIRECT,
    RG_CHECKPOINT_SWAPCHAIN_BLIT,
    RG_CHECKPOINT_SWAPCHAIN_LAYOUT_CHANGE,
    RG_CHECKPOINT_RASTERIZER_BEGIN,
    RG_CHECKPOINT_RASTERIZER_END,
    RG_CHECKPOINT_BLUE_NOISE_UPLOAD,
    RG_CHECKPOINT_VERTEX_COLLECTOR_COPY,
    RG_CHECKPOINT_VERTEX_COLLECTOR_COPY_INDICES,
    RG_CHECKPOINT_END_FRAME
};

#define SET_CHECKPOINT(cmd, c) svkCmdSetCheckpointNV(cmd, (void*)c)

extern VkQueue _graphicsQueue;
void _VkCheckError(VkResult r);

#define VK_CHECKERROR(x) _VkCheckError(x)


#define SET_DEBUG_NAME(device, obj, type, name) if (svkDebugMarkerSetObjectNameEXT != nullptr) AddDebugName(device, reinterpret_cast<uint64_t>(obj), type, name)

// If name is null, debug name won't be set
void AddDebugName(VkDevice device, uint64_t obj, VkDebugReportObjectTypeEXT type, const char *name);