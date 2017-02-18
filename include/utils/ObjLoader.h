#ifndef OBJ_LOADER_H__
#define OBJ_LOADER_H__

#include <glad.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct OBJIndex
{
    unsigned int m_vertexIndex;
    unsigned int m_uvIndex;
    unsigned int m_normalIndex;
    
    bool operator<(const OBJIndex& r) const { return m_vertexIndex < r.m_vertexIndex; }
};

class IndexedModel
{
public:
	void Clear()
	{
		m_positions.clear();
		m_texCoords.clear();
		m_normals.clear();
		m_indices.clear();
	}
    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec2> m_texCoords;
    std::vector<glm::vec3> m_normals;
    std::vector<GLushort> m_indices;
    
    void CalcNormals();
};

class OBJModel
{
public:
    std::vector<OBJIndex> m_OBJIndices;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_uvs;
    std::vector<glm::vec3> m_normals;
    bool m_hasUVs;
    bool m_hasNormals;
    
    OBJModel(const std::string& fileName);
    
    IndexedModel ToIndexedModel();
private:
    unsigned int FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result);
    void CreateOBJFace(const std::string& line);
    
    glm::vec2 ParseOBJVec2(const std::string& line);
    glm::vec3 ParseOBJVec3(const std::string& line);
    OBJIndex ParseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals);
};

#endif 
