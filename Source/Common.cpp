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

#include "Common.h"

#include <vector>

// extension functions' definitions
#define VK_EXTENSION_FUNCTION(fname) PFN_##fname s##fname;
    VK_INSTANCE_DEBUG_UTILS_FUNCTION_LIST
    VK_DEVICE_FUNCTION_LIST
    VK_DEVICE_DEBUG_UTILS_FUNCTION_LIST
#undef VK_EXTENSION_FUNCTION


void InitInstanceExtensionFunctions_DebugUtils(VkInstance instance)
{
    #define VK_EXTENSION_FUNCTION(fname) \
		s##fname = (PFN_##fname)vkGetInstanceProcAddr(instance, #fname); \
		assert(s##fname != nullptr);

    VK_INSTANCE_DEBUG_UTILS_FUNCTION_LIST
    #undef VK_EXTENSION_FUNCTION
}

void InitDeviceExtensionFunctions(VkDevice device)
{
    #define VK_EXTENSION_FUNCTION(fname) \
		s##fname = (PFN_##fname)vkGetDeviceProcAddr(device, #fname); \
		assert(s##fname != nullptr);

    VK_DEVICE_FUNCTION_LIST
    #undef VK_EXTENSION_FUNCTION
}

void InitDeviceExtensionFunctions_DebugUtils(VkDevice device)
{
#define VK_EXTENSION_FUNCTION(fname) \
		s##fname = (PFN_##fname)vkGetDeviceProcAddr(device, #fname); \
		assert(s##fname != nullptr);

    VK_DEVICE_DEBUG_UTILS_FUNCTION_LIST
    #undef VK_EXTENSION_FUNCTION
}

VkQueue _graphicsQueue = VK_NULL_HANDLE;

void _VkCheckError(VkResult r)
{
    if (r == VK_ERROR_DEVICE_LOST)
    {
        uint32_t count;
        svkGetQueueCheckpointDataNV(_graphicsQueue, &count, nullptr);

        uint32_t newCount = count > 4096 ? 4096 : count;

        std::vector<VkCheckpointDataNV> checkpoints(newCount);
        svkGetQueueCheckpointDataNV(_graphicsQueue, &newCount, checkpoints.data());

        std::vector<RgDebugCheckpoints> rgCheckpoints;

        for (auto &c : checkpoints)
        {
            uint64_t data = (uint64_t)c.pCheckpointMarker;

            rgCheckpoints.push_back((RgDebugCheckpoints)data);

            printf("Pipeline stage: %d       Data: %d\n", c.stage, data);
        }

        assert(r == VK_SUCCESS);
    }

    assert(r == VK_SUCCESS);
}

void AddDebugName(VkDevice device, uint64_t obj, VkDebugReportObjectTypeEXT type, const char *name)
{
    if (name == nullptr)
    {
        return;
    }

    VkDebugMarkerObjectNameInfoEXT nameInfo = {};
    nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
    nameInfo.object = obj;
    nameInfo.objectType = type;
    nameInfo.pObjectName = name;

    VkResult r = svkDebugMarkerSetObjectNameEXT(device, &nameInfo);
    VK_CHECKERROR(r);
}
