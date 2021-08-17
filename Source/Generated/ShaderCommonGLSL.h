// This file was generated by GenerateShaderCommon.py

#define MAX_STATIC_VERTEX_COUNT (1048576)
#define MAX_DYNAMIC_VERTEX_COUNT (2097152)
#define MAX_INDEXED_PRIMITIVE_COUNT (1048576)
#define MAX_BOTTOM_LEVEL_GEOMETRIES_COUNT (4096)
#define MAX_BOTTOM_LEVEL_GEOMETRIES_COUNT_POW (12)
#define MAX_GEOMETRY_PRIMITIVE_COUNT (1048576)
#define MAX_GEOMETRY_PRIMITIVE_COUNT_POW (20)
#define LOWER_BOTTOM_LEVEL_GEOMETRIES_COUNT (16)
#define MAX_TOP_LEVEL_INSTANCE_COUNT (45)
#define BINDING_VERTEX_BUFFER_STATIC (0)
#define BINDING_VERTEX_BUFFER_DYNAMIC (1)
#define BINDING_INDEX_BUFFER_STATIC (2)
#define BINDING_INDEX_BUFFER_DYNAMIC (3)
#define BINDING_GEOMETRY_INSTANCES (4)
#define BINDING_GEOMETRY_INSTANCES_MATCH_PREV (5)
#define BINDING_PREV_POSITIONS_BUFFER_DYNAMIC (6)
#define BINDING_PREV_INDEX_BUFFER_DYNAMIC (7)
#define BINDING_GLOBAL_UNIFORM (0)
#define BINDING_ACCELERATION_STRUCTURE_MAIN (0)
#define BINDING_TEXTURES (0)
#define BINDING_CUBEMAPS (0)
#define BINDING_RENDER_CUBEMAP (0)
#define BINDING_BLUE_NOISE (0)
#define BINDING_LUM_HISTOGRAM (0)
#define BINDING_LIGHT_SOURCES_SPHERICAL (0)
#define BINDING_LIGHT_SOURCES_DIRECTIONAL (1)
#define BINDING_LIGHT_SOURCES_SPH_MATCH_PREV (2)
#define BINDING_LIGHT_SOURCES_DIR_MATCH_PREV (3)
#define INSTANCE_CUSTOM_INDEX_FLAG_DYNAMIC (1 << 0)
#define INSTANCE_CUSTOM_INDEX_FLAG_FIRST_PERSON (1 << 1)
#define INSTANCE_CUSTOM_INDEX_FLAG_FIRST_PERSON_VIEWER (1 << 2)
#define INSTANCE_CUSTOM_INDEX_FLAG_REFLECT_REFRACT (1 << 3)
#define INSTANCE_MASK_ALL (0xFF)
#define INSTANCE_MASK_WORLD_MIN (0)
#define INSTANCE_MASK_WORLD_ALL (0x3F)
#define INSTANCE_MASK_WORLD_0 (1 << 0)
#define INSTANCE_MASK_WORLD_1 (1 << 1)
#define INSTANCE_MASK_WORLD_2 (1 << 2)
#define INSTANCE_MASK_WORLD_3 (1 << 3)
#define INSTANCE_MASK_WORLD_4 (1 << 4)
#define INSTANCE_MASK_REFLECT_REFRACT (1 << 5)
#define INSTANCE_MASK_FIRST_PERSON (1 << 6)
#define INSTANCE_MASK_FIRST_PERSON_VIEWER (1 << 7)
#define PAYLOAD_INDEX_DEFAULT (0)
#define PAYLOAD_INDEX_SHADOW (1)
#define SBT_INDEX_RAYGEN_PRIMARY (0)
#define SBT_INDEX_RAYGEN_DIRECT (1)
#define SBT_INDEX_RAYGEN_INDIRECT (2)
#define SBT_INDEX_MISS_DEFAULT (0)
#define SBT_INDEX_MISS_SHADOW (1)
#define SBT_INDEX_HITGROUP_FULLY_OPAQUE (0)
#define SBT_INDEX_HITGROUP_ALPHA_TESTED (1)
#define MATERIAL_ALBEDO_ALPHA_INDEX (0)
#define MATERIAL_ROUGHNESS_METALLIC_EMISSION_INDEX (1)
#define MATERIAL_NORMAL_INDEX (2)
#define MATERIAL_NO_TEXTURE (0)
#define MATERIAL_BLENDING_FLAG_OPAQUE (1 << 0)
#define MATERIAL_BLENDING_FLAG_ALPHA (1 << 1)
#define MATERIAL_BLENDING_FLAG_ADD (1 << 2)
#define MATERIAL_BLENDING_FLAG_SHADE (1 << 3)
#define MATERIAL_BLENDING_FLAG_BIT_COUNT (4)
#define MATERIAL_BLENDING_MASK_FIRST_LAYER (15)
#define MATERIAL_BLENDING_MASK_SECOND_LAYER (240)
#define MATERIAL_BLENDING_MASK_THIRD_LAYER (3840)
#define GEOM_INST_FLAG_REFRACT (1 << 24)
#define GEOM_INST_FLAG_REFLECT (1 << 25)
#define GEOM_INST_FLAG_PORTAL (1 << 26)
#define GEOM_INST_FLAG_MEDIA_TYPE_WATER (1 << 27)
#define GEOM_INST_FLAG_MEDIA_TYPE_GLASS (1 << 28)
#define GEOM_INST_FLAG_GENERATE_NORMALS (1 << 29)
#define GEOM_INST_FLAG_INVERTED_NORMALS (1 << 30)
#define GEOM_INST_FLAG_IS_MOVABLE (1 << 31)
#define SKY_TYPE_COLOR (0)
#define SKY_TYPE_CUBEMAP (1)
#define SKY_TYPE_RASTERIZED_GEOMETRY (2)
#define BLUE_NOISE_TEXTURE_COUNT (128)
#define BLUE_NOISE_TEXTURE_SIZE (128)
#define BLUE_NOISE_TEXTURE_SIZE_POW (7)
#define COMPUTE_COMPOSE_GROUP_SIZE_X (16)
#define COMPUTE_COMPOSE_GROUP_SIZE_Y (16)
#define COMPUTE_BLOOM_UPSAMPLE_GROUP_SIZE_X (16)
#define COMPUTE_BLOOM_UPSAMPLE_GROUP_SIZE_Y (16)
#define COMPUTE_BLOOM_DOWNSAMPLE_GROUP_SIZE_X (16)
#define COMPUTE_BLOOM_DOWNSAMPLE_GROUP_SIZE_Y (16)
#define COMPUTE_BLOOM_STEP_COUNT (5)
#define COMPUTE_LUM_HISTOGRAM_GROUP_SIZE_X (16)
#define COMPUTE_LUM_HISTOGRAM_GROUP_SIZE_Y (16)
#define COMPUTE_LUM_HISTOGRAM_BIN_COUNT (256)
#define COMPUTE_VERT_PREPROC_GROUP_SIZE_X (256)
#define VERT_PREPROC_MODE_ONLY_DYNAMIC (0)
#define VERT_PREPROC_MODE_DYNAMIC_AND_MOVABLE (1)
#define VERT_PREPROC_MODE_ALL (2)
#define COMPUTE_GRADIENT_SAMPLES_GROUP_SIZE_X (16)
#define COMPUTE_GRADIENT_MERGING_GROUP_SIZE_X (16)
#define COMPUTE_GRADIENT_ATROUS_GROUP_SIZE_X (16)
#define COMPUTE_SVGF_TEMPORAL_GROUP_SIZE_X (16)
#define COMPUTE_SVGF_VARIANCE_GROUP_SIZE_X (16)
#define COMPUTE_SVGF_ATROUS_GROUP_SIZE_X (16)
#define COMPUTE_SVGF_ATROUS_ITERATION_COUNT (4)
#define COMPUTE_ASVGF_STRATA_SIZE (3)
#define COMPUTE_ASVGF_GRADIENT_ATROUS_ITERATION_COUNT (4)
#define MAX_RAY_LENGTH (10000.0)
#define MEDIA_TYPE_VACUUM (0)
#define MEDIA_TYPE_WATER (1)
#define MEDIA_TYPE_GLASS (2)
#define MEDIA_TYPE_COUNT (3)



struct ShVertexBufferStatic
{
    float positions[3145728];
    float normals[3145728];
    float texCoords[2097152];
    float texCoordsLayer1[2097152];
    float texCoordsLayer2[2097152];
};

struct ShVertexBufferDynamic
{
    float positions[6291456];
    float normals[6291456];
    float texCoords[4194304];
};

struct ShGlobalUniform
{
    mat4 view;
    mat4 invView;
    mat4 viewPrev;
    mat4 projection;
    mat4 invProjection;
    mat4 projectionPrev;
    uint positionsStride;
    uint normalsStride;
    uint texCoordsStride;
    float renderWidth;
    float renderHeight;
    uint frameId;
    float timeDelta;
    float minLogLuminance;
    float maxLogLuminance;
    float luminanceWhitePoint;
    uint stopEyeAdaptation;
    uint lightCountSpherical;
    uint lightCountDirectional;
    uint skyType;
    float skyColorMultiplier;
    uint skyCubemapIndex;
    vec4 skyColorDefault;
    vec4 skyViewerPosition;
    vec4 cameraPosition;
    uint dbgShowMotionVectors;
    uint dbgShowGradients;
    uint lightCountSphericalPrev;
    uint lightCountDirectionalPrev;
    float emissionMapBoost;
    float emissionMaxScreenColor;
    float normalMapStrength;
    float skyColorSaturation;
    vec4 spotlightPosition;
    vec4 spotlightDirection;
    vec4 spotlightUpVector;
    vec4 spotlightColor;
    float spotlightRadius;
    float spotlightCosAngleOuter;
    float spotlightCosAngleInner;
    float spotlightFalloffDistance;
    uint maxBounceShadowsDirectionalLights;
    uint maxBounceShadowsSphereLights;
    uint maxBounceShadowsSpotlights;
    uint rayCullMaskWorld;
    float bloomThreshold;
    float bloomThresholdLength;
    float bloomUpsampleRadius;
    float bloomIntensity;
    float bloomEmissionMultiplier;
    float bloomSkyMultiplier;
    float rayLength;
    uint reflectRefractMaxDepth;
    uint cameraMediaType;
    float _pad1;
    float _pad2;
    float _pad3;
    vec4 portalInputToOutputDiff;
    ivec4 instanceGeomInfoOffset[12];
    ivec4 instanceGeomInfoOffsetPrev[12];
    ivec4 instanceGeomCount[12];
    mat4 viewProjCubemap[6];
};

struct ShGeometryInstance
{
    mat4 model;
    mat4 prevModel;
    vec4 materialColors[3];
    uint materials0A;
    uint materials0B;
    uint materials0C;
    uint materials1A;
    uint materials1B;
    uint materials1C;
    uint materials2A;
    uint materials2B;
    uint materials2C;
    uint flags;
    uint baseVertexIndex;
    uint baseIndexIndex;
    uint prevBaseVertexIndex;
    uint prevBaseIndexIndex;
    uint vertexCount;
    uint indexCount;
    float defaultRoughness;
    float defaultMetallicity;
    float defaultEmission;
    uint __pad0;
};

struct ShTonemapping
{
    uint histogram[256];
    float avgLuminance;
};

struct ShLightSpherical
{
    vec3 position;
    float radius;
    vec3 color;
    float falloff;
};

struct ShLightDirectional
{
    vec3 direction;
    float tanAngularRadius;
    vec3 color;
    uint __pad0;
};

struct ShVertPreprocessing
{
    uint tlasInstanceCount;
    uint tlasInstanceIsDynamicBits[2];
};

#ifdef DESC_SET_FRAMEBUFFERS

// framebuffers
layout(set = DESC_SET_FRAMEBUFFERS, binding = 0, rgba16f) uniform image2D framebufAlbedo;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 1, r32ui) uniform uimage2D framebufNormal;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 2, r32ui) uniform uimage2D framebufNormal_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 3, r32ui) uniform uimage2D framebufNormalGeometry;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 4, r32ui) uniform uimage2D framebufNormalGeometry_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 5, rgba8) uniform image2D framebufMetallicRoughness;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 6, rgba8) uniform image2D framebufMetallicRoughness_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 7, rgba32f) uniform image2D framebufDepth;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 8, rgba32f) uniform image2D framebufDepth_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 9, r32ui) uniform uimage2D framebufRandomSeed;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 10, r32ui) uniform uimage2D framebufRandomSeed_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 11, r32ui) uniform uimage2D framebufUnfilteredDirect;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 12, r32ui) uniform uimage2D framebufUnfilteredSpecular;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 13, rgba16f) uniform image2D framebufUnfilteredIndirectSH_R;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 14, rgba16f) uniform image2D framebufUnfilteredIndirectSH_G;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 15, rgba16f) uniform image2D framebufUnfilteredIndirectSH_B;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 16, rgba32f) uniform image2D framebufSurfacePosition;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 17, rgba32f) uniform image2D framebufVisibilityBuffer;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 18, rgba32f) uniform image2D framebufVisibilityBuffer_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 19, rgba16f) uniform image2D framebufViewDirection;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 20, rgba16f) uniform image2D framebufThroughput;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 21, r11f_g11f_b10f) uniform image2D framebufPreFinal;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 22, r11f_g11f_b10f) uniform image2D framebufFinal;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 23, rgba16f) uniform image2D framebufMotion;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 24, r11f_g11f_b10f) uniform image2D framebufAccumHistoryLength;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 25, r11f_g11f_b10f) uniform image2D framebufAccumHistoryLength_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 26, r32ui) uniform uimage2D framebufDiffAccumColor;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 27, r32ui) uniform uimage2D framebufDiffAccumColor_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 28, rg16f) uniform image2D framebufDiffAccumMoments;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 29, rg16f) uniform image2D framebufDiffAccumMoments_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 30, rgba16f) uniform image2D framebufDiffColorHistory;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 31, rgba16f) uniform image2D framebufDiffPingColorAndVariance;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 32, rgba16f) uniform image2D framebufDiffPongColorAndVariance;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 33, r32ui) uniform uimage2D framebufSpecAccumColor;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 34, r32ui) uniform uimage2D framebufSpecAccumColor_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 35, r32ui) uniform uimage2D framebufSpecPingColor;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 36, r32ui) uniform uimage2D framebufSpecPongColor;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 37, rgba16f) uniform image2D framebufIndirAccumSH_R;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 38, rgba16f) uniform image2D framebufIndirAccumSH_R_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 39, rgba16f) uniform image2D framebufIndirAccumSH_G;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 40, rgba16f) uniform image2D framebufIndirAccumSH_G_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 41, rgba16f) uniform image2D framebufIndirAccumSH_B;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 42, rgba16f) uniform image2D framebufIndirAccumSH_B_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 43, rgba16f) uniform image2D framebufIndirPingSH_R;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 44, rgba16f) uniform image2D framebufIndirPingSH_G;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 45, rgba16f) uniform image2D framebufIndirPingSH_B;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 46, rgba16f) uniform image2D framebufIndirPongSH_R;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 47, rgba16f) uniform image2D framebufIndirPongSH_G;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 48, rgba16f) uniform image2D framebufIndirPongSH_B;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 49, r16f) uniform image2D framebufAtrousFilteredVariance;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 50, rgba32ui) uniform uimage2D framebufGradientSamples;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 51, rgba32ui) uniform uimage2D framebufGradientSamples_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 52, rgba16f) uniform image2D framebufDiffAndSpecPingGradient;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 53, rgba16f) uniform image2D framebufDiffAndSpecPongGradient;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 54, r16f) uniform image2D framebufIndirPingGradient;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 55, r16f) uniform image2D framebufIndirPongGradient;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 56, r11f_g11f_b10f) uniform image2D framebufBloom_Mip1;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 57, r11f_g11f_b10f) uniform image2D framebufBloom_Mip2;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 58, r11f_g11f_b10f) uniform image2D framebufBloom_Mip3;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 59, r11f_g11f_b10f) uniform image2D framebufBloom_Mip4;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 60, r11f_g11f_b10f) uniform image2D framebufBloom_Mip5;

// samplers
layout(set = DESC_SET_FRAMEBUFFERS, binding = 61) uniform sampler2D framebufAlbedo_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 62) uniform usampler2D framebufNormal_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 63) uniform usampler2D framebufNormal_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 64) uniform usampler2D framebufNormalGeometry_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 65) uniform usampler2D framebufNormalGeometry_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 66) uniform sampler2D framebufMetallicRoughness_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 67) uniform sampler2D framebufMetallicRoughness_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 68) uniform sampler2D framebufDepth_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 69) uniform sampler2D framebufDepth_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 70) uniform usampler2D framebufRandomSeed_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 71) uniform usampler2D framebufRandomSeed_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 72) uniform usampler2D framebufUnfilteredDirect_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 73) uniform usampler2D framebufUnfilteredSpecular_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 74) uniform sampler2D framebufUnfilteredIndirectSH_R_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 75) uniform sampler2D framebufUnfilteredIndirectSH_G_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 76) uniform sampler2D framebufUnfilteredIndirectSH_B_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 77) uniform sampler2D framebufSurfacePosition_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 78) uniform sampler2D framebufVisibilityBuffer_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 79) uniform sampler2D framebufVisibilityBuffer_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 80) uniform sampler2D framebufViewDirection_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 81) uniform sampler2D framebufThroughput_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 82) uniform sampler2D framebufPreFinal_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 83) uniform sampler2D framebufFinal_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 84) uniform sampler2D framebufMotion_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 85) uniform sampler2D framebufAccumHistoryLength_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 86) uniform sampler2D framebufAccumHistoryLength_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 87) uniform usampler2D framebufDiffAccumColor_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 88) uniform usampler2D framebufDiffAccumColor_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 89) uniform sampler2D framebufDiffAccumMoments_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 90) uniform sampler2D framebufDiffAccumMoments_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 91) uniform sampler2D framebufDiffColorHistory_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 92) uniform sampler2D framebufDiffPingColorAndVariance_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 93) uniform sampler2D framebufDiffPongColorAndVariance_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 94) uniform usampler2D framebufSpecAccumColor_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 95) uniform usampler2D framebufSpecAccumColor_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 96) uniform usampler2D framebufSpecPingColor_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 97) uniform usampler2D framebufSpecPongColor_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 98) uniform sampler2D framebufIndirAccumSH_R_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 99) uniform sampler2D framebufIndirAccumSH_R_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 100) uniform sampler2D framebufIndirAccumSH_G_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 101) uniform sampler2D framebufIndirAccumSH_G_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 102) uniform sampler2D framebufIndirAccumSH_B_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 103) uniform sampler2D framebufIndirAccumSH_B_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 104) uniform sampler2D framebufIndirPingSH_R_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 105) uniform sampler2D framebufIndirPingSH_G_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 106) uniform sampler2D framebufIndirPingSH_B_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 107) uniform sampler2D framebufIndirPongSH_R_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 108) uniform sampler2D framebufIndirPongSH_G_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 109) uniform sampler2D framebufIndirPongSH_B_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 110) uniform sampler2D framebufAtrousFilteredVariance_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 111) uniform usampler2D framebufGradientSamples_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 112) uniform usampler2D framebufGradientSamples_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 113) uniform sampler2D framebufDiffAndSpecPingGradient_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 114) uniform sampler2D framebufDiffAndSpecPongGradient_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 115) uniform sampler2D framebufIndirPingGradient_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 116) uniform sampler2D framebufIndirPongGradient_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 117) uniform sampler2D framebufBloom_Mip1_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 118) uniform sampler2D framebufBloom_Mip2_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 119) uniform sampler2D framebufBloom_Mip3_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 120) uniform sampler2D framebufBloom_Mip4_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 121) uniform sampler2D framebufBloom_Mip5_Sampler;

// pack/unpack formats
void imageStoreUnfilteredDirect(const ivec2 pix, const vec3 unpacked) { imageStore(framebufUnfilteredDirect, pix, uvec4(encodeE5B9G9R9(unpacked))); }
vec3 texelFetchUnfilteredDirect(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufUnfilteredDirect_Sampler, pix, 0).r); }

void imageStoreUnfilteredSpecular(const ivec2 pix, const vec3 unpacked) { imageStore(framebufUnfilteredSpecular, pix, uvec4(encodeE5B9G9R9(unpacked))); }
vec3 texelFetchUnfilteredSpecular(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufUnfilteredSpecular_Sampler, pix, 0).r); }

void imageStoreDiffAccumColor(const ivec2 pix, const vec3 unpacked) { imageStore(framebufDiffAccumColor, pix, uvec4(encodeE5B9G9R9(unpacked))); }
vec3 texelFetchDiffAccumColor(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufDiffAccumColor_Sampler, pix, 0).r); }
vec3 texelFetchDiffAccumColor_Prev(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufDiffAccumColor_Prev_Sampler, pix, 0).r); }

void imageStoreSpecAccumColor(const ivec2 pix, const vec3 unpacked) { imageStore(framebufSpecAccumColor, pix, uvec4(encodeE5B9G9R9(unpacked))); }
vec3 texelFetchSpecAccumColor(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufSpecAccumColor_Sampler, pix, 0).r); }
vec3 texelFetchSpecAccumColor_Prev(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufSpecAccumColor_Prev_Sampler, pix, 0).r); }

void imageStoreSpecPingColor(const ivec2 pix, const vec3 unpacked) { imageStore(framebufSpecPingColor, pix, uvec4(encodeE5B9G9R9(unpacked))); }
vec3 texelFetchSpecPingColor(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufSpecPingColor_Sampler, pix, 0).r); }

void imageStoreSpecPongColor(const ivec2 pix, const vec3 unpacked) { imageStore(framebufSpecPongColor, pix, uvec4(encodeE5B9G9R9(unpacked))); }
vec3 texelFetchSpecPongColor(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufSpecPongColor_Sampler, pix, 0).r); }


#endif
