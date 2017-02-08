#include "mesh.h"
#include "util.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

bool MeshManager::m_initialized = false;
MeshManager *MeshManager::m_instance = 0;

static std::map<TileType, std::string> tiletype_lookup
{
	{ TileType::BLACK, "res\\black.robj" },
	{ TileType::BRICKS, "res\\bricks.robj" },
	{ TileType::DESERT, "res\\desert.robj" },
	{ TileType::EMMY, "res\\emmy.robj" },
	{ TileType::FAIRY, "res\\fairy.robj" },
	{ TileType::GRASS, "res\\grass.robj" },
	{ TileType::GRASS2, "res\\grass2.robj" },
	{ TileType::JELLYBEAN, "res\\jellybean.robj" },
	{ TileType::MAINCHAR, "res\\mainchar.robj" },
	{ TileType::MERMAID, "res\\mermaid.robj" },
	{ TileType::MTN, "res\\mtn.robj" },
	{ TileType::MTNSNOW, "res\\mtnsnow.robj" },
	{ TileType::MUD, "res\\mud.robj" },
	{ TileType::OCTOPUS, "res\\octopus.robj" },
	{ TileType::PARCHMENT, "res\\parchment.robj" },
	{ TileType::PLANK, "res\\plank.robj" },
	{ TileType::PRINCESS, "res\\princess.robj" },
	{ TileType::ROCKS, "res\\rocks.robj" },
	{ TileType::ROOF, "res\\roof.robj" },
	{ TileType::WATER, "res\\water.robj" },
	{ TileType::WOOD, "res\\wood.robj" }
};

bool GetPrefabModel(const std::string &str, IndexedModel &dat);

MeshManager *MeshManager::GetInstance()
{
	if (m_initialized)
	{
		return m_instance;
	}

	m_instance = new MeshManager();
	m_initialized = true;
	return m_instance;
}

bool MeshManager::GetMeshPtr(TileType type, Mesh **mesh)
{
	auto iter = tiletype_lookup.find(type);
	if (iter != tiletype_lookup.end() && iter->second != "")
	{
		if (GetMeshPtr(iter->second, mesh))
		{
			return true;
		}
	}
	return false;
}

bool MeshManager::GetMeshPtr(const std::string &filename, Mesh **mesh)
{
	auto iter2 = m_lookup.find(filename);
	if (iter2 != m_lookup.end())
	{
		*mesh = iter2->second;
		return true;
	}

	*mesh = new Mesh(filename);
	if (*mesh == nullptr) return false;
	m_lookup[filename] = *mesh;
	return true;
}

Mesh::Mesh(const std::string& fileName)
{
	if (GetPrefabModel(fileName, m_model))
	{
		InitMesh(m_model);
	}
	else
	{
		m_model = OBJModel(fileName).ToIndexedModel();
		InitMesh(m_model);
	}
	m_texture = nullptr;
	m_num_instances = 0;
	m_color = RGB(1, 1, 1);
}

Mesh::Mesh(IndexedModel &model, bool calcNormalsForMe)
{
   if (calcNormalsForMe) model.CalcNormals();
   InitMesh(model);
   m_texture = nullptr;
   m_num_instances = 0;
   m_color = RGB(1, 1, 1);
}

bool Mesh::UseTexture(TileType &ttype)
{
   TextureManager *tm = TextureManager::GetInstance();
   tm->GetTexturePtr(ttype, &m_texture);
   if (m_texture != nullptr) return true;
   return false;
}

bool Mesh::UseTexture(std::string &texture_filename)
{
   if (texture_filename.size() == 0) return false;
   TextureManager *tm = TextureManager::GetInstance();
   tm->GetTexturePtr(texture_filename, &m_texture);
   if (m_texture != nullptr) return true;
   return false;
}

void Mesh::SetUpInstancing(int n_instances, glm::vec3 scalevect, glm::mat4 *translations)
{
	glBindVertexArray(m_VAO);
	if (m_VBO_ids.size() < 5)
	{
		GLuint tmp;
		glGenBuffers(1, &tmp);
		m_VBO_ids.push_back(tmp);
	}
   m_num_instances = n_instances;
   
   for (int aa = 0; aa < n_instances; aa++)
   {
	   translations[aa] = glm::scale(translations[aa], scalevect);
   }

   glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::TRANS_IDX)]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*n_instances, translations, GL_STATIC_DRAW);

	for (int i = 0; i < 4; ++i)
	{
		glVertexAttribPointer(static_cast<int>(AttributeIdx::TRANSLATION_LOC) + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid *)(i * sizeof(glm::vec4)));
		glVertexAttribDivisor(static_cast<int>(AttributeIdx::TRANSLATION_LOC) + i, 1);
		glEnableVertexAttribArray(static_cast<int>(AttributeIdx::TRANSLATION_LOC) + i);
	}
	glBindVertexArray(0);
}

void Mesh::InitMesh(const IndexedModel& model)
{
	m_numIndices = model.indices.size();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	
	GLuint tmp;
	glGenBuffers(1, &tmp);
	m_VBO_ids.push_back(tmp);
	glGenBuffers(1, &tmp);
	m_VBO_ids.push_back(tmp);
	glGenBuffers(1, &tmp);
	m_VBO_ids.push_back(tmp);
	glGenBuffers(1, &tmp);
	m_VBO_ids.push_back(tmp);
	
	// Spatial Positions (-1 to 1)
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::POS_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(static_cast<int>(AttributeIdx::POS_LOC));
	glVertexAttribPointer(static_cast<int>(AttributeIdx::POS_LOC), 3, GL_FLOAT, GL_FALSE, 0, 0);
		
	// Tri-Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::IDX_IDX)]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	// Texture Coords
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::TEXTURE_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(static_cast<int>(AttributeIdx::TEXTURE_LOC));
	glVertexAttribPointer(static_cast<int>(AttributeIdx::TEXTURE_LOC), 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::NORMAL_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(static_cast<int>(AttributeIdx::NORMAL_LOC));
	glVertexAttribPointer(static_cast<int>(AttributeIdx::NORMAL_LOC), 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	
}

void Mesh::Render()
{
   glBindVertexArray(m_VAO);

   if (m_texture != nullptr) m_texture->Bind();

   if (m_num_instances > 0)
   {
	   glDrawElementsInstanced(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_SHORT, NULL, m_num_instances);
	   glFlush();
   }
   else
   {
      glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
   }
   
   if (m_texture != nullptr) m_texture->UnBind();
   glBindVertexArray(0);
}

bool GetPrefabModel(const std::string &str, IndexedModel &dat)
{
	if (str == "res\\mtn.robj" || str == "res\\mtnsnow.robj")
	{
		dat.Clear();
		dat.positions.emplace_back(glm::vec3(-.5f, -.5, 0)); //LL
		dat.positions.emplace_back(glm::vec3(.5f, -.5, 0)); //LR
		dat.positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE

		dat.positions.emplace_back(glm::vec3(.5f, -.5, 0)); //LR
		dat.positions.emplace_back(glm::vec3(.5f, .5, 0)); //UR
		dat.positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE

		dat.positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE
		dat.positions.emplace_back(glm::vec3(.5f, .5, 0)); //UR
		dat.positions.emplace_back(glm::vec3(-.5f, .5, 0)); //UL

		dat.positions.emplace_back(glm::vec3(-.5f, -.5, 0)); //LL
		dat.positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE
		dat.positions.emplace_back(glm::vec3(-.5f, .5, 0)); //UL

		dat.texCoords.emplace_back(0, 0); //LL
		dat.texCoords.emplace_back(1, 0); //LR
		dat.texCoords.emplace_back(0, 0); //C

		dat.texCoords.emplace_back(1, 0); //LR
		dat.texCoords.emplace_back(1, 1); //UR
		dat.texCoords.emplace_back(0, 0); //C

		dat.texCoords.emplace_back(0, 0); //C
		dat.texCoords.emplace_back(1, 1); //UR
		dat.texCoords.emplace_back(0, 1); //UL

		dat.texCoords.emplace_back(0, 0); //LL
		dat.texCoords.emplace_back(0, 0); //C
		dat.texCoords.emplace_back(0, 1); //UL

		dat.indices.emplace_back(0);
		dat.indices.emplace_back(1);
		dat.indices.emplace_back(2);
		dat.indices.emplace_back(3);
		dat.indices.emplace_back(4);
		dat.indices.emplace_back(5);
		dat.indices.emplace_back(6);
		dat.indices.emplace_back(7);
		dat.indices.emplace_back(8);
		dat.indices.emplace_back(9);
		dat.indices.emplace_back(10);
		dat.indices.emplace_back(11);

		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		return true;
	}
	else if ( str == "res\\emmy.robj" || str == "res\\fairy.robj" ||
              str == "res\\jellybean.robj" || str == "res\\mainchar.robj" ||
		      str == "res\\mermaid.robj" || str == "res\\octopus.robj" ||
              str == "res\\princess.robj")
	{ 
		dat.Clear();
		dat.Clear();
		dat.positions.emplace_back(glm::vec3(-.5f, -.5, 0)); //LL
		dat.positions.emplace_back(glm::vec3(.5f, -.5, 0)); //LR
		dat.positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE

		dat.positions.emplace_back(glm::vec3(.5f, -.5, 0)); //LR
		dat.positions.emplace_back(glm::vec3(.5f, .5, 0)); //UR
		dat.positions.emplace_back(glm::vec3(0, 0, -.1)); //CENTRE

		dat.positions.emplace_back(glm::vec3(0, 0, -.1)); //CENTRE
		dat.positions.emplace_back(glm::vec3(.5f, .5, 0)); //UR
		dat.positions.emplace_back(glm::vec3(-.5f, .5, 0)); //UL

		dat.positions.emplace_back(glm::vec3(-.5f, -.5, 0)); //LL
		dat.positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE
		dat.positions.emplace_back(glm::vec3(-.5f, .5, 0)); //UL

		dat.texCoords.emplace_back(0, 0); //LL
		dat.texCoords.emplace_back(1, 0); //LR
		dat.texCoords.emplace_back(0, 0); //C

		dat.texCoords.emplace_back(1, 0); //LR
		dat.texCoords.emplace_back(1, 1); //UR
		dat.texCoords.emplace_back(0, 0); //C

		dat.texCoords.emplace_back(0, 0); //C
		dat.texCoords.emplace_back(1, 1); //UR
		dat.texCoords.emplace_back(0, 1); //UL

		dat.texCoords.emplace_back(0, 0); //LL
		dat.texCoords.emplace_back(0, 0); //C
		dat.texCoords.emplace_back(0, 1); //UL

		dat.indices.emplace_back(0);
		dat.indices.emplace_back(1);
		dat.indices.emplace_back(2);
		dat.indices.emplace_back(3);
		dat.indices.emplace_back(4);
		dat.indices.emplace_back(5);
		dat.indices.emplace_back(6);
		dat.indices.emplace_back(7);
		dat.indices.emplace_back(8);
		dat.indices.emplace_back(9);
		dat.indices.emplace_back(10);
		dat.indices.emplace_back(11);

		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		return true;
	}
	else 
	{
		dat.Clear();
		dat.positions.emplace_back(glm::vec3(-.5f, -.5, 0)); //LL
		dat.positions.emplace_back(glm::vec3(.5f, -.5, 0)); //LR
		dat.positions.emplace_back(glm::vec3(.5f, .5, 0)); //UR
		dat.positions.emplace_back(glm::vec3(-.5f, .5, 0)); //UL
     
		dat.texCoords.emplace_back(0, 0); //LL
		dat.texCoords.emplace_back(1, 0); //LR
		dat.texCoords.emplace_back(1, 1); //UR
		dat.texCoords.emplace_back(0, 1); //UL
		
		dat.indices.emplace_back(0);
		dat.indices.emplace_back(1);
		dat.indices.emplace_back(2);
		dat.indices.emplace_back(0);
		dat.indices.emplace_back(2);
		dat.indices.emplace_back(3);
			
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		return true;
	}
	/*else if (str == "res\\pyramid.robj")
	{
		dat.Clear();
		dat.positions.emplace_back(glm::vec3(-1, 0, 2)); 
		dat.positions.emplace_back(glm::vec3(1, 0, 2));
		dat.positions.emplace_back(glm::vec3(1, 0, 0)); 
		dat.positions.emplace_back(glm::vec3(-1, 0, 0)); 
		dat.positions.emplace_back(glm::vec3(0, 1, 1)); 
		
		dat.texCoords.emplace_back(0, .5); //LL
		dat.texCoords.emplace_back(1, .5); //LR
		dat.texCoords.emplace_back(1, .5); //C
		dat.texCoords.emplace_back(0, .5); //LR
		dat.texCoords.emplace_back(.5, 1); //UR
		
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		dat.normals.emplace_back(glm::vec3(0, 0, -1));
		
		dat.indices.emplace_back(0);
		dat.indices.emplace_back(1);
		dat.indices.emplace_back(4);
		dat.indices.emplace_back(1);
		dat.indices.emplace_back(2);
		dat.indices.emplace_back(4);
		dat.indices.emplace_back(2);
		dat.indices.emplace_back(3);
		dat.indices.emplace_back(4);
		dat.indices.emplace_back(3);
		dat.indices.emplace_back(0);
		dat.indices.emplace_back(4);

		return true;
	}
	*/
	return false;
}
