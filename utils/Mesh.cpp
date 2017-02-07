#include "mesh.h"
#include "util.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

bool MeshManager::m_instance_created = false;
MeshManager *MeshManager::m_instance = 0;

static std::map<TileType, std::string> tiletype_lookup
{
	{ TileType::BLACK, "res\\flat.robj" },
	{ TileType::BRICKS, "res\\flat.robj" },
	{ TileType::DESERT, "res\\flat.robj" },
	{ TileType::EMMY, "res\\flat.robj" },
	{ TileType::FAIRY, "res\\flat.robj" },
	{ TileType::GRASS, "res\\flat.robj" },
	{ TileType::GRASS2, "res\\flat.robj" },
	{ TileType::JELLYBEAN, "res\\flat.robj" },
	{ TileType::MAINCHAR, "res\\flat.robj" },
	{ TileType::MERMAID, "res\\flat.robj" },
	{ TileType::MTN, "res\\pyramid.robj" },
	{ TileType::MTNSNOW, "res\\mtn.robj" },
	{ TileType::MUD, "res\\flat.robj" },
	{ TileType::OCTOPUS, "res\\flat.robj" },
	{ TileType::PARCHMENT, "res\\flat.robj" },
	{ TileType::PLANK, "res\\flat.robj" },
	{ TileType::PRINCESS, "res\\flat.robj" },
	{ TileType::ROCKS, "res\\flat.robj" },
	{ TileType::ROOF, "res\\flat.robj" },
	{ TileType::WATER, "res\\flat.robj" },
	{ TileType::WOOD, "res\\flat.robj" }
};

bool GetPrefabModel(const std::string &str, IndexedModel &dat);

MeshManager *MeshManager::GetInstance()
{
	if (m_instance_created)
	{
		return m_instance;
	}

	m_instance = new MeshManager();
	m_instance_created = true;
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
	m_instancing_enabled = false;
}

Mesh::Mesh(IndexedModel &model, bool calcNormalsForMe)
{
	if (calcNormalsForMe) model.CalcNormals();
	InitMesh(model);
	m_texture = nullptr;
	m_instancing_enabled = false;
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

static double get_rand(int boundy)
{
	static bool is_seeded(false);
	if (!is_seeded)
	{
		srand(14);
		is_seeded = true;
	}
	int result = rand() % boundy;
	return result;
}

void Mesh::InitMesh(const IndexedModel& model)
{
	m_numIndices = model.indices.size();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	
	glGenBuffers(4, m_VBO_ids);
	
	if ((sizeof(GLfloat) * 3) != sizeof(glm::vec3))
	{
		std::cout << "potential sizing problem" << std::endl;
	}

	// Spatial Positions (-1 to 1)
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::POS_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(static_cast<int>(AttributeIdx::POS_LOC));
	glVertexAttribPointer(static_cast<int>(AttributeIdx::POS_LOC), 3, GL_FLOAT, GL_FALSE, 0, 0);
		
	// Tri-Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::IDX_IDX)]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

#define NINST 12

	static GLfloat *colors = nullptr;
	if (colors == nullptr)
	{
		// Note: this 256 is the number of tile instances we will draw
		colors = new GLfloat[3 * NINST]; //256 instances * 3colors per instance
		for (int aa = 0; aa < 3 * NINST; aa++)
		{
			colors[aa] = get_rand(256) / 256.0;
		}
	}
	
	// Colors
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::COLOR_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * NINST , colors, GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<int>(AttributeIdx::COLOR_LOC), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(static_cast<int>(AttributeIdx::COLOR_LOC), 1);
	glEnableVertexAttribArray(static_cast<int>(AttributeIdx::COLOR_LOC));

	static glm::mat4 *translations = nullptr;
	if(translations ==  nullptr)
	{
		translations = new glm::mat4[NINST];
		translations[0] = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -3.0f, -14.0f));
		translations[1] = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -1.0f, -17.0f));
		translations[2] = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -2.0f, -9.0f));
		for (int bb = 3; bb < NINST; bb++)
		{
		   translations[bb] = glm::translate(glm::mat4(1.0f), glm::vec3(get_rand(10)-5, get_rand(10)-5, get_rand(20)-20));
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::TRANS_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*NINST, translations, GL_STATIC_DRAW);

	for (int i = 0; i < 4; ++i)
	{
		glVertexAttribPointer(static_cast<int>(AttributeIdx::TRANSLATION_LOC) + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid *)(i * sizeof(glm::vec4)));
		glVertexAttribDivisor(static_cast<int>(AttributeIdx::TRANSLATION_LOC) + i, 1);
		glEnableVertexAttribArray(static_cast<int>(AttributeIdx::TRANSLATION_LOC) + i);
	}

	glBindVertexArray(0);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[MeshBufferPositionsNORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::TEXTURE_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	*/
}

Mesh::~Mesh()
{
	
}

void Mesh::Render()
{
	RenderInstanced();
	/*
	glColor3f(static_cast<GLfloat> (1),	static_cast<GLfloat> (1),
		static_cast<GLfloat> (1));
	
	if (m_texture != nullptr) m_texture->Bind();
	
	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	//glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);
	//glDrawArrays(GL_TRIANGLES,0,3);

	glBindVertexArray(0);
	if (m_texture != nullptr)  m_texture->UnBind();
	*/
}

void Mesh::RenderInstanced()
{
	//if (m_texture != nullptr) m_texture->Bind();

	glBindVertexArray(m_VAO);

	glDrawElementsInstanced(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_SHORT, NULL, NINST);
		
	//glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);
	//glDrawArrays(GL_TRIANGLES,0,3);

	glFlush();
	//glBindVertexArray(0);
	//if (m_texture != nullptr)  m_texture->UnBind();
}

bool GetPrefabModel(const std::string &str, IndexedModel &dat)
{
	if (str == "res\\mtn.robj")
	{
		dat.Clear();
		dat.positions.emplace_back(glm::vec3(-.5f, -1, 0)); //LL
		dat.positions.emplace_back(glm::vec3(.5f, -1, 0)); //LR
		dat.positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE

		dat.positions.emplace_back(glm::vec3(.5f, -1, 0)); //LR
		dat.positions.emplace_back(glm::vec3(.5f, 1, 0)); //UR
		dat.positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE

		dat.positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE
		dat.positions.emplace_back(glm::vec3(.5f, 1, 0)); //UR
		dat.positions.emplace_back(glm::vec3(-.5f, 1, 0)); //UL

		dat.positions.emplace_back(glm::vec3(-.5f, -1, 0)); //LL
		dat.positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE
		dat.positions.emplace_back(glm::vec3(-.5f, 1, 0)); //UL

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
	else if (str == "res\\flat.robj")
	{
		dat.Clear();
		dat.positions.emplace_back(glm::vec3(-.5f, -1, 0)); //LL
		dat.positions.emplace_back(glm::vec3(.5f, -1, 0)); //LR
		dat.positions.emplace_back(glm::vec3(.5f, -1, 0)); //LR
		dat.positions.emplace_back(glm::vec3(.5f, 1, 0)); //UR
		dat.positions.emplace_back(glm::vec3(.5f, 1, 0)); //UR
		dat.positions.emplace_back(glm::vec3(-.5f, 1, 0)); //UL
     	dat.positions.emplace_back(glm::vec3(-.5f, -1, 0)); //LL
		dat.positions.emplace_back(glm::vec3(-.5f, 1, 0)); //UL

		dat.texCoords.emplace_back(0, 0); //LL
		dat.texCoords.emplace_back(1, 0); //LR
		dat.texCoords.emplace_back(1, 0); //LR
		dat.texCoords.emplace_back(1, 1); //UR
		dat.texCoords.emplace_back(1, 1); //UR
		dat.texCoords.emplace_back(0, 1); //UL
		dat.texCoords.emplace_back(0, 0); //LL
		dat.texCoords.emplace_back(0, 1); //UL

		dat.indices.emplace_back(0);
		dat.indices.emplace_back(1);
		dat.indices.emplace_back(2);
		dat.indices.emplace_back(3);
		dat.indices.emplace_back(4);
		dat.indices.emplace_back(5);
		dat.indices.emplace_back(6);
		dat.indices.emplace_back(7);
	
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
	else if (str == "res\\pyramid.robj")
	{
		dat.Clear();
		dat.positions.emplace_back(glm::vec3(-1, 0, 2)); 
		dat.positions.emplace_back(glm::vec3(1, 0, 2));
		dat.positions.emplace_back(glm::vec3(1, 0, 0)); 
		dat.positions.emplace_back(glm::vec3(-1, 0, 0)); 
		dat.positions.emplace_back(glm::vec3(0, 1, 1)); 
		
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
	return false;
}