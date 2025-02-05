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

#ifdef DESC_SET_GLOBAL_UNIFORM
#ifdef DESC_SET_VERTEX_DATA
layout(
    set = DESC_SET_VERTEX_DATA,
    binding = BINDING_VERTEX_BUFFER_STATIC)
    #ifndef VERTEX_BUFFER_WRITEABLE
    readonly 
    #endif
    buffer VertexBufferStatic_BT
{
    ShVertexBufferStatic staticVertices;
};

layout(
    set = DESC_SET_VERTEX_DATA,
    binding = BINDING_VERTEX_BUFFER_DYNAMIC)
    #ifndef VERTEX_BUFFER_WRITEABLE
    readonly 
    #endif
    buffer VertexBufferDynamic_BT
{
    ShVertexBufferDynamic dynamicVertices;
};

layout(
    set = DESC_SET_VERTEX_DATA,
    binding = BINDING_INDEX_BUFFER_STATIC)
    readonly 
    buffer IndexBufferStatic_BT
{
    uint staticIndices[];
};

layout(
    set = DESC_SET_VERTEX_DATA,
    binding = BINDING_INDEX_BUFFER_DYNAMIC)
    readonly 
    buffer IndexBufferDynamic_BT
{
    uint dynamicIndices[];
};

layout(
    set = DESC_SET_VERTEX_DATA,
    binding = BINDING_GEOMETRY_INSTANCES)
    readonly 
    buffer GeometryInstances_BT
{
    ShGeometryInstance geometryInstances[];
};

layout(
    set = DESC_SET_VERTEX_DATA,
    binding = BINDING_GEOMETRY_INSTANCES_MATCH_PREV)
    readonly 
    buffer GeometryIndicesPrevToCur_BT
{
    int geomIndexPrevToCur[];
};

layout(
    set = DESC_SET_VERTEX_DATA,
    binding = BINDING_PREV_POSITIONS_BUFFER_DYNAMIC)
    #ifndef VERTEX_BUFFER_WRITEABLE
    readonly 
    #endif
    buffer PrevPositionsBufferStatic_BT
{
    float prevDynamicPositions[];
};

layout(
    set = DESC_SET_VERTEX_DATA,
    binding = BINDING_PREV_INDEX_BUFFER_DYNAMIC)
    #ifndef VERTEX_BUFFER_WRITEABLE
    readonly 
    #endif
    buffer PrevIndexBufferDynamic_BT
{
    uint prevDynamicIndices[];
};

layout(
    set = DESC_SET_VERTEX_DATA,
    binding = BINDING_PER_TRIANGLE_INFO)
    readonly 
    buffer PerTriangleInfo_BT
{
    uint triangleSectorIndices[];
};

vec3 getStaticVerticesPositions(uint index)
{
    return vec3(
        staticVertices.positions[index * globalUniform.positionsStride + 0],
        staticVertices.positions[index * globalUniform.positionsStride + 1],
        staticVertices.positions[index * globalUniform.positionsStride + 2]);
}

vec3 getStaticVerticesNormals(uint index)
{
    return vec3(
        staticVertices.normals[index * globalUniform.normalsStride + 0],
        staticVertices.normals[index * globalUniform.normalsStride + 1],
        staticVertices.normals[index * globalUniform.normalsStride + 2]);
}

vec2 getStaticVerticesTexCoords(uint index)
{
    return vec2(
        staticVertices.texCoords[index * globalUniform.texCoordsStride + 0],
        staticVertices.texCoords[index * globalUniform.texCoordsStride + 1]);
}

vec2 getStaticVerticesTexCoordsLayer1(uint index)
{
    return vec2(
        staticVertices.texCoordsLayer1[index * globalUniform.texCoordsStride + 0],
        staticVertices.texCoordsLayer1[index * globalUniform.texCoordsStride + 1]);
}

vec2 getStaticVerticesTexCoordsLayer2(uint index)
{
    return vec2(
        staticVertices.texCoordsLayer2[index * globalUniform.texCoordsStride + 0],
        staticVertices.texCoordsLayer2[index * globalUniform.texCoordsStride + 1]);
}

vec3 getDynamicVerticesPositions(uint index)
{
    return vec3(
        dynamicVertices.positions[index * globalUniform.positionsStride + 0],
        dynamicVertices.positions[index * globalUniform.positionsStride + 1],
        dynamicVertices.positions[index * globalUniform.positionsStride + 2]);
}

vec3 getDynamicVerticesNormals(uint index)
{
    return vec3(
        dynamicVertices.normals[index * globalUniform.normalsStride + 0],
        dynamicVertices.normals[index * globalUniform.normalsStride + 1],
        dynamicVertices.normals[index * globalUniform.normalsStride + 2]);
}

vec2 getDynamicVerticesTexCoords(uint index)
{
    return vec2(
        dynamicVertices.texCoords[index * globalUniform.texCoordsStride + 0],
        dynamicVertices.texCoords[index * globalUniform.texCoordsStride + 1]);
}

#ifdef VERTEX_BUFFER_WRITEABLE
void setStaticVerticesPositions(uint index, vec3 value)
{
    staticVertices.positions[index * globalUniform.positionsStride + 0] = value[0];
    staticVertices.positions[index * globalUniform.positionsStride + 1] = value[1];
    staticVertices.positions[index * globalUniform.positionsStride + 2] = value[2];
}

void setStaticVerticesNormals(uint index, vec3 value)
{
    staticVertices.normals[index * globalUniform.normalsStride + 0] = value[0];
    staticVertices.normals[index * globalUniform.normalsStride + 1] = value[1];
    staticVertices.normals[index * globalUniform.normalsStride + 2] = value[2];
}

void setStaticVerticesTexCoords(uint index, vec2 value)
{
    staticVertices.texCoords[index * globalUniform.texCoordsStride + 0] = value[0];
    staticVertices.texCoords[index * globalUniform.texCoordsStride + 1] = value[1];
}

void setStaticVerticesTexCoordsLayer1(uint index, vec2 value)
{
    staticVertices.texCoordsLayer1[index * globalUniform.texCoordsStride + 0] = value[0];
    staticVertices.texCoordsLayer1[index * globalUniform.texCoordsStride + 1] = value[1];
}

void setStaticVerticesTexCoordsLayer2(uint index, vec2 value)
{
    staticVertices.texCoordsLayer2[index * globalUniform.texCoordsStride + 0] = value[0];
    staticVertices.texCoordsLayer2[index * globalUniform.texCoordsStride + 1] = value[1];
}

void setDynamicVerticesPositions(uint index, vec3 value)
{
    dynamicVertices.positions[index * globalUniform.positionsStride + 0] = value[0];
    dynamicVertices.positions[index * globalUniform.positionsStride + 1] = value[1];
    dynamicVertices.positions[index * globalUniform.positionsStride + 2] = value[2];
}

void setDynamicVerticesNormals(uint index, vec3 value)
{
    dynamicVertices.normals[index * globalUniform.normalsStride + 0] = value[0];
    dynamicVertices.normals[index * globalUniform.normalsStride + 1] = value[1];
    dynamicVertices.normals[index * globalUniform.normalsStride + 2] = value[2];
}

void setDynamicVerticesTexCoords(uint index, vec2 value)
{
    dynamicVertices.texCoords[index * globalUniform.texCoordsStride + 0] = value[0];
    dynamicVertices.texCoords[index * globalUniform.texCoordsStride + 1] = value[1];
}
#endif // VERTEX_BUFFER_WRITEABLE

// Get indices in vertex buffer. If geom uses index buffer then it flattens them to vertex buffer indices.
uvec3 getVertIndicesStatic(uint baseVertexIndex, uint baseIndexIndex, uint primitiveId)
{
    // if to use indices
    if (baseIndexIndex != UINT32_MAX)
    {
        return uvec3(
            baseVertexIndex + staticIndices[baseIndexIndex + primitiveId * 3 + 0],
            baseVertexIndex + staticIndices[baseIndexIndex + primitiveId * 3 + 1],
            baseVertexIndex + staticIndices[baseIndexIndex + primitiveId * 3 + 2]);
    }
    else
    {
        return uvec3(
            baseVertexIndex + primitiveId * 3 + 0,
            baseVertexIndex + primitiveId * 3 + 1,
            baseVertexIndex + primitiveId * 3 + 2);
    }
}

uvec3 getVertIndicesDynamic(uint baseVertexIndex, uint baseIndexIndex, uint primitiveId)
{
    // if to use indices
    if (baseIndexIndex != UINT32_MAX)
    {
        return uvec3(
            baseVertexIndex + dynamicIndices[baseIndexIndex + primitiveId * 3 + 0],
            baseVertexIndex + dynamicIndices[baseIndexIndex + primitiveId * 3 + 1],
            baseVertexIndex + dynamicIndices[baseIndexIndex + primitiveId * 3 + 2]);
    }
    else
    {
        return uvec3(
            baseVertexIndex + primitiveId * 3 + 0,
            baseVertexIndex + primitiveId * 3 + 1,
            baseVertexIndex + primitiveId * 3 + 2);
    }
}

// Only for dynamic, static geom vertices are not changed.
uvec3 getPrevVertIndicesDynamic(uint prevBaseVertexIndex, uint prevBaseIndexIndex, uint primitiveId)
{
    // if to use indices
    if (prevBaseIndexIndex != UINT32_MAX)
    {
        return uvec3(
            prevBaseVertexIndex + prevDynamicIndices[prevBaseIndexIndex + primitiveId * 3 + 0],
            prevBaseVertexIndex + prevDynamicIndices[prevBaseIndexIndex + primitiveId * 3 + 1],
            prevBaseVertexIndex + prevDynamicIndices[prevBaseIndexIndex + primitiveId * 3 + 2]);
    }
    else
    {
        return uvec3(
            prevBaseVertexIndex + primitiveId * 3 + 0,
            prevBaseVertexIndex + primitiveId * 3 + 1,
            prevBaseVertexIndex + primitiveId * 3 + 2);
    }
}

vec3 getPrevDynamicVerticesPositions(uint index)
{
    return vec3(
        prevDynamicPositions[index * globalUniform.positionsStride + 0],
        prevDynamicPositions[index * globalUniform.positionsStride + 1],
        prevDynamicPositions[index * globalUniform.positionsStride + 2]);
}

vec4 getTangent(const mat3 localPos, const vec3 normal, const mat3x2 texCoord)
{
    const vec3 e1 = localPos[1] - localPos[0];
    const vec3 e2 = localPos[2] - localPos[0];

    const vec2 u1 = texCoord[1] - texCoord[0];
    const vec2 u2 = texCoord[2] - texCoord[0];

    const float invDet = 1.0 / (u1.x * u2.y - u2.x * u1.y);

    const vec3 tangent   = normalize((e1 * u2.y - e2 * u1.y) * invDet);
    const vec3 bitangent = normalize((e2 * u1.x - e1 * u2.x) * invDet);

    // Don't store bitangent, only store cross(normal, tangent) handedness.
    // If that cross product and bitangent have the same sign,
    // then handedness is 1, otherwise -1
    float handedness = float(dot(cross(normal, tangent), bitangent) > 0.0);
    handedness = handedness * 2.0 - 1.0;

    return vec4(tangent, handedness);
}

ShTriangle getTriangleStatic(uvec3 vertIndices, uint baseVertexIndex, uint baseIndexIndex, uint primitiveId)
{
    ShTriangle tr;

    tr.positions[0] = getStaticVerticesPositions(vertIndices[0]);
    tr.positions[1] = getStaticVerticesPositions(vertIndices[1]);
    tr.positions[2] = getStaticVerticesPositions(vertIndices[2]);

    tr.normals[0] = getStaticVerticesNormals(vertIndices[0]);
    tr.normals[1] = getStaticVerticesNormals(vertIndices[1]);
    tr.normals[2] = getStaticVerticesNormals(vertIndices[2]);

    tr.layerTexCoord[0][0] = getStaticVerticesTexCoords(vertIndices[0]);
    tr.layerTexCoord[0][1] = getStaticVerticesTexCoords(vertIndices[1]);
    tr.layerTexCoord[0][2] = getStaticVerticesTexCoords(vertIndices[2]);

    tr.layerTexCoord[1][0] = getStaticVerticesTexCoordsLayer1(vertIndices[0]);
    tr.layerTexCoord[1][1] = getStaticVerticesTexCoordsLayer1(vertIndices[1]);
    tr.layerTexCoord[1][2] = getStaticVerticesTexCoordsLayer1(vertIndices[2]);

    tr.layerTexCoord[2][0] = getStaticVerticesTexCoordsLayer2(vertIndices[0]);
    tr.layerTexCoord[2][1] = getStaticVerticesTexCoordsLayer2(vertIndices[1]);
    tr.layerTexCoord[2][2] = getStaticVerticesTexCoordsLayer2(vertIndices[2]);

    // get very coarse normal for triangle to determine bitangent's handedness
    tr.tangent = getTangent(tr.positions, safeNormalize(tr.normals[0] + tr.normals[1] + tr.normals[2]), tr.layerTexCoord[0]);

    return tr;
}

ShTriangle getTriangleDynamic(uvec3 vertIndices, uint baseVertexIndex, uint baseIndexIndex, uint primitiveId)
{
    ShTriangle tr;

    tr.positions[0] = getDynamicVerticesPositions(vertIndices[0]);
    tr.positions[1] = getDynamicVerticesPositions(vertIndices[1]);
    tr.positions[2] = getDynamicVerticesPositions(vertIndices[2]);

    tr.normals[0] = getDynamicVerticesNormals(vertIndices[0]);
    tr.normals[1] = getDynamicVerticesNormals(vertIndices[1]);
    tr.normals[2] = getDynamicVerticesNormals(vertIndices[2]);

    tr.layerTexCoord[0][0] = getDynamicVerticesTexCoords(vertIndices[0]);
    tr.layerTexCoord[0][1] = getDynamicVerticesTexCoords(vertIndices[1]);
    tr.layerTexCoord[0][2] = getDynamicVerticesTexCoords(vertIndices[2]);

    // get very coarse normal for triangle to determine bitangent's handedness
    tr.tangent = getTangent(tr.positions, safeNormalize(tr.normals[0] + tr.normals[1] + tr.normals[2]), tr.layerTexCoord[0]);

    return tr;
}

// Get geometry index in "geometryInstances" array by instanceID, localGeometryIndex.
int getGeometryIndex(int instanceID, int localGeometryIndex)
{
    return globalUniform.instanceGeomInfoOffset[instanceID / 4][instanceID % 4] + localGeometryIndex;
}

bool getCurrentGeometryIndexByPrev(int prevInstanceID, int prevLocalGeometryIndex, out int curFrameGlobalGeomIndex)
{
    // get previous frame's global geom index
    const int prevFrameGeomIndex = globalUniform.instanceGeomInfoOffsetPrev[prevInstanceID / 4][prevInstanceID % 4] + prevLocalGeometryIndex;
    
    // try to find global geom index in current frame by it
    curFrameGlobalGeomIndex = geomIndexPrevToCur[prevFrameGeomIndex];

    // UINT32_MAX -- no prev to cur exist
    return curFrameGlobalGeomIndex != UINT32_MAX;
}

/*#define GEOMETRY_INSTANCE_TEXTURE_SET_SIZE 3

uvec3 getGeometryInstanceMaterialLayer(const ShGeometryInstance inst, int layer)
{
    return uvec3(
        inst.materials[layer * GEOMETRY_INSTANCE_TEXTURE_SET_SIZE + MATERIAL_ALBEDO_ALPHA_INDEX], 
        inst.materials[layer * GEOMETRY_INSTANCE_TEXTURE_SET_SIZE + MATERIAL_ROUGHNESS_METALLIC_EMISSION_INDEX], 
        inst.materials[layer * GEOMETRY_INSTANCE_TEXTURE_SET_SIZE + MATERIAL_NORMAL_INDEX]
    );
}*/

// localGeometryIndex is index of geometry in pGeometries in BLAS
// primitiveId is index of a triangle
ShTriangle getTriangle(int instanceID, int instanceCustomIndex, int localGeometryIndex, int primitiveId)
{
    ShTriangle tr;

    // get info about geometry by the index in pGeometries in BLAS with index "instanceID"
    const int globalGeometryIndex = getGeometryIndex(instanceID, localGeometryIndex);
    const ShGeometryInstance inst = geometryInstances[globalGeometryIndex];

    const bool isDynamic = (instanceCustomIndex & INSTANCE_CUSTOM_INDEX_FLAG_DYNAMIC) == INSTANCE_CUSTOM_INDEX_FLAG_DYNAMIC;

    if (isDynamic)
    {
        const uvec3 vertIndices = getVertIndicesDynamic(inst.baseVertexIndex, inst.baseIndexIndex, primitiveId);

        tr = getTriangleDynamic(vertIndices, inst.baseVertexIndex, inst.baseIndexIndex, primitiveId);

        // only one material for dynamic geometry
        tr.materials[0] = uvec3(inst.materials0A, inst.materials0B, inst.materials0C);
        tr.materials[1] = uvec3(MATERIAL_NO_TEXTURE);
        tr.materials[2] = uvec3(MATERIAL_NO_TEXTURE);
        
        tr.materialColors[0] = inst.materialColors[0];

        // to world space
        tr.positions[0] = (inst.model * vec4(tr.positions[0], 1.0)).xyz;
        tr.positions[1] = (inst.model * vec4(tr.positions[1], 1.0)).xyz;
        tr.positions[2] = (inst.model * vec4(tr.positions[2], 1.0)).xyz;
        
        // dynamic     -- use prev model matrix and prev positions if exist
        const bool hasPrevInfo = inst.prevBaseVertexIndex != UINT32_MAX;

        if (hasPrevInfo)
        {
            const uvec3 prevVertIndices = getPrevVertIndicesDynamic(inst.prevBaseVertexIndex, inst.prevBaseIndexIndex, primitiveId);

            const vec4 prevLocalPos[] =
            {
                vec4(getPrevDynamicVerticesPositions(prevVertIndices[0]), 1.0),
                vec4(getPrevDynamicVerticesPositions(prevVertIndices[1]), 1.0),
                vec4(getPrevDynamicVerticesPositions(prevVertIndices[2]), 1.0)
            };

            tr.prevPositions[0] = (inst.prevModel * prevLocalPos[0]).xyz;
            tr.prevPositions[1] = (inst.prevModel * prevLocalPos[1]).xyz;
            tr.prevPositions[2] = (inst.prevModel * prevLocalPos[2]).xyz;
        }
        else
        {
            tr.prevPositions[0] = tr.positions[0];
            tr.prevPositions[1] = tr.positions[1];
            tr.prevPositions[2] = tr.positions[2];
        }
    }
    else
    {
        const uvec3 vertIndices = getVertIndicesStatic(inst.baseVertexIndex, inst.baseIndexIndex, primitiveId);

        tr = getTriangleStatic(vertIndices, inst.baseVertexIndex, inst.baseIndexIndex, primitiveId);

        tr.materials[0] = uvec3(inst.materials0A, inst.materials0B, inst.materials0C);
        tr.materials[1] = uvec3(inst.materials1A, inst.materials1B, inst.materials1C);
        tr.materials[2] = uvec3(inst.materials2A, inst.materials2B, MATERIAL_NO_TEXTURE);

        tr.materialColors[0] = inst.materialColors[0];
        tr.materialColors[1] = inst.materialColors[1];
        tr.materialColors[2] = inst.materialColors[2];

        const vec4 prevLocalPos[] =
        {
            vec4(tr.positions[0], 1.0),
            vec4(tr.positions[1], 1.0),
            vec4(tr.positions[2], 1.0),
        };

        // to world space
        tr.positions[0] = (inst.model * prevLocalPos[0]).xyz;
        tr.positions[1] = (inst.model * prevLocalPos[1]).xyz;
        tr.positions[2] = (inst.model * prevLocalPos[2]).xyz;
        
        const bool isMovable = (inst.flags & GEOM_INST_FLAG_IS_MOVABLE) != 0;
        const bool hasPrevInfo = inst.prevBaseVertexIndex != UINT32_MAX;

        // movable     -- use prev model matrix if exist
        // non-movable -- use current model matrix
        if (isMovable && hasPrevInfo)
        {
            // static geoms' local positions are constant, 
            // only model matrices are changing
            tr.prevPositions[0] = (inst.prevModel * prevLocalPos[0]).xyz;
            tr.prevPositions[1] = (inst.prevModel * prevLocalPos[1]).xyz;
            tr.prevPositions[2] = (inst.prevModel * prevLocalPos[2]).xyz;
        }
        else
        {
            tr.prevPositions[0] = tr.positions[0];
            tr.prevPositions[1] = tr.positions[1];
            tr.prevPositions[2] = tr.positions[2];
        }
    }
    
    const mat3 model3 = mat3(inst.model);

    // to world space
    tr.normals[0] = model3 * tr.normals[0];
    tr.normals[1] = model3 * tr.normals[1];
    tr.normals[2] = model3 * tr.normals[2];
    tr.tangent.xyz = model3 * tr.tangent.xyz;


    tr.geometryInstanceFlags = inst.flags;

    tr.geomRoughness = inst.defaultRoughness;
    tr.geomMetallicity = inst.defaultMetallicity;

    // use (first layer's color) * defaultEmission
    tr.geomEmission = inst.defaultEmission;

    // if should use per-triangle info
    if (inst.triangleArrayIndex != GEOM_INST_NO_TRIANGLE_INFO)
    {
        tr.sectorArrayIndex = triangleSectorIndices[inst.triangleArrayIndex + primitiveId];
    }
    else
    {
        tr.sectorArrayIndex = inst.sectorArrayIndex;
    }

    return tr;
}

// Copy of getTriangle but only get surf pos/normal info
void getTriangle_PositionAndNormal(
    int instanceID, int instanceCustomIndex, int localGeometryIndex, int primitiveId, const vec2 bary,
    out vec3 position, out vec3 position_Prev,
    out vec3 normal, out vec3 normal_Prev)
{
    const vec3 baryCoords = vec3(1.0 - bary.x - bary.y, bary.x, bary.y);

    ShTriangle tr;

    // get info about geometry by the index in pGeometries in BLAS with index "instanceID"
    const int globalGeometryIndex = getGeometryIndex(instanceID, localGeometryIndex);
    const ShGeometryInstance inst = geometryInstances[globalGeometryIndex];

    const bool isDynamic = (instanceCustomIndex & INSTANCE_CUSTOM_INDEX_FLAG_DYNAMIC) == INSTANCE_CUSTOM_INDEX_FLAG_DYNAMIC;

    if (isDynamic)
    {
        const uvec3 vertIndices = getVertIndicesDynamic(inst.baseVertexIndex, inst.baseIndexIndex, primitiveId);

        tr = getTriangleDynamic(vertIndices, inst.baseVertexIndex, inst.baseIndexIndex, primitiveId);

        // to local and then to world space
        position = tr.positions * baryCoords;
        position = (inst.model * vec4(position, 1.0)).xyz;

        const vec3 localNormal = tr.normals * baryCoords;
        normal = mat3(inst.model) * localNormal;
        
        
        // dynamic     -- use prev model matrix and prev positions if exist
        const bool hasPrevInfo = inst.prevBaseVertexIndex != UINT32_MAX;

        if (hasPrevInfo)
        {
            const uvec3 prevVertIndices = getPrevVertIndicesDynamic(inst.prevBaseVertexIndex, inst.prevBaseIndexIndex, primitiveId);

            position_Prev =
                getPrevDynamicVerticesPositions(prevVertIndices[0]) * baryCoords[0] +
                getPrevDynamicVerticesPositions(prevVertIndices[1]) * baryCoords[1] +
                getPrevDynamicVerticesPositions(prevVertIndices[2]) * baryCoords[2];
                
            position_Prev = (inst.prevModel * vec4(position_Prev, 1.0)).xyz;

            // TODO: prev normals array, like with prevDynamicPositions
            normal_Prev = mat3(inst.prevModel) * localNormal;
        }
        else
        {
            position_Prev = position;
            normal_Prev = normal;
        }
    }
    else
    {
        const uvec3 vertIndices = getVertIndicesStatic(inst.baseVertexIndex, inst.baseIndexIndex, primitiveId);

        tr = getTriangleStatic(vertIndices, inst.baseVertexIndex, inst.baseIndexIndex, primitiveId);

        // to local and then to world space
        const vec3 localPosition = tr.positions * baryCoords;
        const vec3 localNormal = tr.normals * baryCoords;
        
        position = (inst.model * vec4(localPosition, 1.0)).xyz;
        normal = mat3(inst.model) * localNormal;

        
        const bool isMovable = (inst.flags & GEOM_INST_FLAG_IS_MOVABLE) != 0;
        const bool hasPrevInfo = inst.prevBaseVertexIndex != UINT32_MAX;

        // movable     -- use prev model matrix if exist
        // non-movable -- use current model matrix
        if (isMovable && hasPrevInfo)
        {
            // static geoms' local positions are constant, 
            // only model matrices are changing
            position_Prev = (inst.prevModel * vec4(localPosition, 1.0)).xyz;
            normal_Prev = mat3(inst.prevModel) * localNormal;
        }
        else
        {
            position_Prev = position;
            normal_Prev = normal;
        }
    }
}

mat3 getOnlyCurPositions(int globalGeometryIndex, int instanceCustomIndex, int primitiveId)
{
    mat3 positions;

    const ShGeometryInstance inst = geometryInstances[globalGeometryIndex];

    const bool isDynamic = (instanceCustomIndex & INSTANCE_CUSTOM_INDEX_FLAG_DYNAMIC) == INSTANCE_CUSTOM_INDEX_FLAG_DYNAMIC;

    if (isDynamic)
    {
        const uvec3 vertIndices = getVertIndicesDynamic(inst.baseVertexIndex, inst.baseIndexIndex, primitiveId);

        // to world space
        positions[0] = (inst.model * vec4(getDynamicVerticesPositions(vertIndices[0]), 1.0)).xyz;
        positions[1] = (inst.model * vec4(getDynamicVerticesPositions(vertIndices[1]), 1.0)).xyz;
        positions[2] = (inst.model * vec4(getDynamicVerticesPositions(vertIndices[2]), 1.0)).xyz;
    }
    else
    {
        const uvec3 vertIndices = getVertIndicesStatic(inst.baseVertexIndex, inst.baseIndexIndex, primitiveId);

        // to world space
        positions[0] = (inst.model * vec4(getStaticVerticesPositions(vertIndices[0]), 1.0)).xyz;
        positions[1] = (inst.model * vec4(getStaticVerticesPositions(vertIndices[1]), 1.0)).xyz;
        positions[2] = (inst.model * vec4(getStaticVerticesPositions(vertIndices[2]), 1.0)).xyz;
    }
    
    return positions;
}

mat3 getOnlyPrevPositions(int globalGeometryIndex, int instanceCustomIndex, int primitiveId)
{
    mat3 prevPositions;

    const ShGeometryInstance inst = geometryInstances[globalGeometryIndex];

    const bool isDynamic = (instanceCustomIndex & INSTANCE_CUSTOM_INDEX_FLAG_DYNAMIC) == INSTANCE_CUSTOM_INDEX_FLAG_DYNAMIC;

    if (isDynamic)
    {
        // dynamic     -- use prev model matrix and prev positions if exist
        const bool hasPrevInfo = inst.prevBaseVertexIndex != UINT32_MAX;

        if (hasPrevInfo)
        {
            const uvec3 prevVertIndices = getPrevVertIndicesDynamic(inst.prevBaseVertexIndex, inst.prevBaseIndexIndex, primitiveId);

            const vec4 prevLocalPos[] =
            {
                vec4(getPrevDynamicVerticesPositions(prevVertIndices[0]), 1.0),
                vec4(getPrevDynamicVerticesPositions(prevVertIndices[1]), 1.0),
                vec4(getPrevDynamicVerticesPositions(prevVertIndices[2]), 1.0)
            };

            prevPositions[0] = (inst.prevModel * prevLocalPos[0]).xyz;
            prevPositions[1] = (inst.prevModel * prevLocalPos[1]).xyz;
            prevPositions[2] = (inst.prevModel * prevLocalPos[2]).xyz;
        }
        else
        {
            const uvec3 vertIndices = getVertIndicesDynamic(inst.baseVertexIndex, inst.baseIndexIndex, primitiveId);
            
            const vec4 localPos[] =
            {
                vec4(getDynamicVerticesPositions(vertIndices[0]), 1.0),
                vec4(getDynamicVerticesPositions(vertIndices[1]), 1.0),
                vec4(getDynamicVerticesPositions(vertIndices[2]), 1.0)
            };

            prevPositions[0] = (inst.model * localPos[0]).xyz;
            prevPositions[1] = (inst.model * localPos[1]).xyz;
            prevPositions[2] = (inst.model * localPos[2]).xyz;
        }
    }
    else
    {
        const uvec3 vertIndices = getVertIndicesStatic(inst.baseVertexIndex, inst.baseIndexIndex, primitiveId);
        
        const vec4 localPos[] =
        {
            vec4(getStaticVerticesPositions(vertIndices[0]), 1.0),
            vec4(getStaticVerticesPositions(vertIndices[1]), 1.0),
            vec4(getStaticVerticesPositions(vertIndices[2]), 1.0),
        };

        const bool isMovable = (inst.flags & GEOM_INST_FLAG_IS_MOVABLE) != 0;
        const bool hasPrevInfo = inst.prevBaseVertexIndex != UINT32_MAX;

        // movable     -- use prev model matrix if exist
        // non-movable -- use current model matrix
        if (isMovable && hasPrevInfo)
        {
            // static geoms' local positions are constant, 
            // only model matrices are changing
            prevPositions[0] = (inst.prevModel * localPos[0]).xyz;
            prevPositions[1] = (inst.prevModel * localPos[1]).xyz;
            prevPositions[2] = (inst.prevModel * localPos[2]).xyz;
        }
        else
        {
            prevPositions[0] = (inst.model * localPos[0]).xyz;
            prevPositions[1] = (inst.model * localPos[1]).xyz;
            prevPositions[2] = (inst.model * localPos[2]).xyz;
        }
    }

    return prevPositions;
}

vec4 packVisibilityBuffer(const ShPayload p)
{
    return vec4(uintBitsToFloat(p.instIdAndIndex), uintBitsToFloat(p.geomAndPrimIndex), p.baryCoords);
}

int unpackInstCustomIndexFromVisibilityBuffer(const vec4 v)
{
    int instanceID, instCustomIndex;
    unpackInstanceIdAndCustomIndex(floatBitsToUint(v[0]), instanceID, instCustomIndex);

    return instCustomIndex;
}

void unpackVisibilityBuffer(
    const vec4 v,
    out int instanceID, out int instCustomIndex,
    out int localGeomIndex, out int primIndex,
    out vec2 bary)
{
    unpackInstanceIdAndCustomIndex(floatBitsToUint(v[0]), instanceID, instCustomIndex);
    unpackGeometryAndPrimitiveIndex(floatBitsToUint(v[1]), localGeomIndex, primIndex);
    bary = vec2(v[2], v[3]);
}

// v must be fetched from framebufVisibilityBuffer_Prev_Sampler
bool unpackPrevVisibilityBuffer(const vec4 v, out vec3 prevPos)
{
    int prevInstanceID, instCustomIndex;
    int prevLocalGeomIndex, primIndex;

    unpackInstanceIdAndCustomIndex(floatBitsToUint(v[0]), prevInstanceID, instCustomIndex);
    unpackGeometryAndPrimitiveIndex(floatBitsToUint(v[1]), prevLocalGeomIndex, primIndex);

    int curFrameGlobalGeomIndex;
    const bool matched = getCurrentGeometryIndexByPrev(prevInstanceID, prevLocalGeomIndex, curFrameGlobalGeomIndex);

    if (!matched)
    {
        return false;
    }

    const mat3 prevVerts = getOnlyCurPositions(curFrameGlobalGeomIndex, instCustomIndex, primIndex);
    const vec3 baryCoords = vec3(1.0 - v[2] - v[3], v[2], v[3]);

    prevPos = prevVerts * baryCoords;

    return true;
}

mat4 getModelMatrix(int instanceID, int localGeometryIndex)
{
    int globalGeometryIndex = getGeometryIndex(instanceID, localGeometryIndex);
    return geometryInstances[globalGeometryIndex].model;
}
#endif // DESC_SET_VERTEX_DATA
#endif // DESC_SET_GLOBAL_UNIFORM