#include "ObjLoader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b);
static inline size_t FindNextChar(size_t start, const char* str, size_t length, char token);
static inline size_t ParseOBJIndexValue(const std::string& token, size_t start, size_t end);
static inline float ParseOBJFloatValue(const std::string& token, size_t start, size_t end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);

OBJModel::OBJModel(const std::string& fileName)
{
   m_hasUVs = false;
   m_hasNormals = false;
    std::ifstream file;
    file.open(fileName.c_str());

    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
        
            size_t lineLength = line.length();
            
            if(lineLength < 2)
                continue;
            
            const char* lineCStr = line.c_str();
            
            switch(lineCStr[0])
            {
                case 'v':
                    if(lineCStr[1] == 't')
                        this->m_uvs.emplace_back(ParseOBJVec2(line));
                    else if(lineCStr[1] == 'n')
                        this->m_normals.emplace_back(ParseOBJVec3(line));
                    else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
                        this->m_vertices.emplace_back(ParseOBJVec3(line));
                break;
                case 'f':
                    CreateOBJFace(line);
                break;
                default: break;
            };
        }
    }
    else
    {
        std::cerr << "Unable to load mesh: " << fileName << std::endl;
    }
}

void IndexedModel::CalcNormals()
{
	if (m_normals.size() != m_positions.size() )
	{
		m_normals.resize(m_positions.size());
	}

	if (m_indices.size() == 0)
	{
		m_indices.resize(m_positions.size());
		for (int aa = 0; aa < m_indices.size(); aa++)
		{
			m_indices[aa] = aa;
		}
	}

    for(size_t i = 0; i < m_indices.size(); i += 3)
    {
        int i0 = m_indices[i];
        int i1 = m_indices[i + 1];
        int i2 = m_indices[i + 2];

        glm::vec3 v1 = m_positions[i1] - m_positions[i0];
        glm::vec3 v2 = m_positions[i2] - m_positions[i0];
        
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
            
        m_normals[i0] += normal;
        m_normals[i1] += normal;
        m_normals[i2] += normal;
    }
    
    for(size_t i = 0; i < m_positions.size(); i++)
        m_normals[i] = glm::normalize(m_normals[i]);
}

IndexedModel OBJModel::ToIndexedModel()
{
    IndexedModel result;
    IndexedModel normalModel;
    
    size_t numIndices = m_OBJIndices.size();

	if (numIndices == 0 && 
		m_vertices.size() > 0)
	{
		// I sneakily short circuit everything if I am loading my homebrew robj file
		bool do_text = (m_vertices.size() == m_uvs.size());
		bool do_norm = (m_vertices.size() == m_normals.size());

		for (int aa = 0; aa < m_vertices.size(); aa++)
		{
			result.m_positions.emplace_back(m_vertices[aa]);
			if(do_text)	result.m_texCoords.emplace_back(m_uvs[aa]);
			if (do_norm)
			{
				result.m_normals.emplace_back(m_normals[aa]);
			}
			else
			{
				result.m_normals.emplace_back(glm::vec3(0, 0, 1));
			}
		}
		
		return result;
	}
	    
    std::vector<OBJIndex*> indexLookup;
    
    for(size_t i = 0; i < numIndices; i++)
        indexLookup.emplace_back(&m_OBJIndices[i]);
    
    std::sort(indexLookup.begin(), indexLookup.end(), CompareOBJIndexPtr);
    
    std::map<OBJIndex, size_t> normalModelIndexMap;
    std::map<size_t, size_t> indexMap;
    
    for(size_t i = 0; i < numIndices; i++)
    {
        OBJIndex* currentIndex = &m_OBJIndices[i];
        
        glm::vec3 currentPosition = m_vertices[currentIndex->m_vertexIndex];
        glm::vec2 currentTexCoord;
        glm::vec3 currentNormal;
        
        if(m_hasUVs)
            currentTexCoord = m_uvs[currentIndex->m_uvIndex];
        else
            currentTexCoord = glm::vec2(0,0);
            
        if(m_hasNormals)
            currentNormal = m_normals[currentIndex->m_normalIndex];
        else
            currentNormal = glm::vec3(0,0,1);
        
        size_t normalModelIndex;
        size_t resultModelIndex;
        
        //Create model to properly generate m_normals on
        std::map<OBJIndex, size_t>::iterator it = normalModelIndexMap.find(*currentIndex);
        if(it == normalModelIndexMap.end())
        {
            normalModelIndex = static_cast<int> (normalModel.m_positions.size());
        
            normalModelIndexMap.insert(std::pair<OBJIndex, size_t>(*currentIndex, normalModelIndex));
            normalModel.m_positions.emplace_back(currentPosition);
            normalModel.m_texCoords.emplace_back(currentTexCoord);
            normalModel.m_normals.emplace_back(currentNormal);
        }
        else
            normalModelIndex = it->second;
        
        //Create model which properly separates texture coordinates
        size_t previousVertexLocation = FindLastVertexIndex(indexLookup, currentIndex, result);
        
        if(previousVertexLocation == (size_t)-1)
        {
            resultModelIndex = static_cast<int>(result.m_positions.size());
        
            result.m_positions.emplace_back(currentPosition);
            result.m_texCoords.emplace_back(currentTexCoord);
            result.m_normals.emplace_back(currentNormal);
        }
        else
            resultModelIndex = previousVertexLocation;
        
        normalModel.m_indices.emplace_back(normalModelIndex);
        result.m_indices.emplace_back(resultModelIndex);
        indexMap.insert(std::pair<size_t, size_t>(resultModelIndex, normalModelIndex));
    }
    
    if(!m_hasNormals)
    {
        normalModel.CalcNormals();
        
        for(size_t i = 0; i < result.m_positions.size(); i++)
            result.m_normals[i] = normalModel.m_normals[indexMap[i]];
    }
    
    return result;
};

size_t OBJModel::FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result)
{
    size_t start = 0;
    size_t end = indexLookup.size();
    size_t current = (end - start) / 2 + start;
    size_t previous = start;
    
    while(current != previous)
    {
        OBJIndex* testIndex = indexLookup[current];
        
        if(testIndex->m_vertexIndex == currentIndex->m_vertexIndex)
        {
            size_t countStart = current;
        
            for(size_t i = 0; i < current; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current - i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->m_vertexIndex != currentIndex->m_vertexIndex)
                    break;
                    
                countStart--;
            }
            
            for(size_t i = countStart; i < indexLookup.size() - countStart; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current + i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->m_vertexIndex != currentIndex->m_vertexIndex)
                    break;
                else if((!m_hasUVs || possibleIndex->m_uvIndex == currentIndex->m_uvIndex) 
                    && (!m_hasNormals || possibleIndex->m_normalIndex == currentIndex->m_normalIndex))
                {
                    glm::vec3 currentPosition = m_vertices[currentIndex->m_vertexIndex];
                    glm::vec2 currentTexCoord;
                    glm::vec3 currentNormal;
                    
                    if(m_hasUVs)
                        currentTexCoord = m_uvs[currentIndex->m_uvIndex];
                    else
                        currentTexCoord = glm::vec2(0,0);
                        
                    if(m_hasNormals)
                        currentNormal = m_normals[currentIndex->m_normalIndex];
                    else
                        currentNormal = glm::vec3(0,0,0);
                    
                    for(size_t j = 0; j < result.m_positions.size(); j++)
                    {
                        if(currentPosition == result.m_positions[j] 
                            && ((!m_hasUVs || currentTexCoord == result.m_texCoords[j])
                            && (!m_hasNormals || currentNormal == result.m_normals[j])))
                        {
                            return j;
                        }
                    }
                }
            }
        
            return -1;
        }
        else
        {
            if(testIndex->m_vertexIndex < currentIndex->m_vertexIndex)
                start = current;
            else
                end = current;
        }
    
        previous = current;
        current = (end - start) / 2 + start;
    }
    
    return -1;
}

void OBJModel::CreateOBJFace(const std::string& line)
{
    std::vector<std::string> tokens = SplitString(line, ' ');

    this->m_OBJIndices.emplace_back(ParseOBJIndex(tokens[1], &this->m_hasUVs, &this->m_hasNormals));
    this->m_OBJIndices.emplace_back(ParseOBJIndex(tokens[2], &this->m_hasUVs, &this->m_hasNormals));
    this->m_OBJIndices.emplace_back(ParseOBJIndex(tokens[3], &this->m_hasUVs, &this->m_hasNormals));

    if((int)tokens.size() > 4)
    {
        this->m_OBJIndices.emplace_back(ParseOBJIndex(tokens[1], &this->m_hasUVs, &this->m_hasNormals));
        this->m_OBJIndices.emplace_back(ParseOBJIndex(tokens[3], &this->m_hasUVs, &this->m_hasNormals));
        this->m_OBJIndices.emplace_back(ParseOBJIndex(tokens[4], &this->m_hasUVs, &this->m_hasNormals));
    }
}

OBJIndex OBJModel::ParseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals)
{
    size_t tokenLength = token.length();
    const char* tokenString = token.c_str();
    
    size_t vertIndexStart = 0;
    size_t vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    OBJIndex result;
    result.m_vertexIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    result.m_uvIndex = 0;
    result.m_normalIndex = 0;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.m_uvIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasUVs = true;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.m_normalIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasNormals = true;
    
    return result;
}

glm::vec3 OBJModel::ParseOBJVec3(const std::string& line) 
{
    size_t tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    size_t vertIndexStart = 2;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    size_t vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec3(x,y,z);

    //glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

glm::vec2 OBJModel::ParseOBJVec2(const std::string& line)
{
    size_t tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    size_t vertIndexStart = 3;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    size_t vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec2(x,y);
}

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b)
{
    return a->m_vertexIndex < b->m_vertexIndex;
}

static inline size_t FindNextChar(size_t start, const char* str, size_t length, char token)
{
    size_t result = start;
    while(result < length)
    {
        result++;
        if(str[result] == token)
            break;
    }
    
    return result;
}

static inline size_t ParseOBJIndexValue(const std::string& token, size_t start, size_t end)
{
    return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, size_t start, size_t end)
{
    return static_cast<float>(atof(token.substr(start, end - start).c_str()));
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;
        
    const char* cstr = s.c_str();
    size_t strLength = s.length();
    size_t start = 0;
    size_t end = 0;
        
    while(end <= strLength)
    {
        while(end <= strLength)
        {
            if(cstr[end] == delim)
                break;
            end++;
        }
            
        elems.emplace_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }
        
    return elems;
}
