#include "mesh.h"
#include "util.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>

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
	{ TileType::MTN, "res\\mtn.robj" },
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
	IndexedModel tmp_model;
	if (GetPrefabModel(fileName, tmp_model))
	{
		InitMesh(tmp_model);
	}
	else
	{
		InitMesh(OBJModel(fileName).ToIndexedModel());
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

void Mesh::InitializeInstancing(glm::vec3 *translations, int n_instances)
{
	if (!m_instancing_enabled)
	{
		GLuint vboid;
		glBindVertexArray(m_vertexArrayObject);

		glGenBuffers(1, &vboid);
		m_VBO_ids.emplace_back(vboid);
		m_instancing_enabled = true;

		glBindBuffer(GL_ARRAY_BUFFER, vboid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * n_instances , translations, GL_STATIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glVertexAttribDivisor(4, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glBindVertexArray(0);
   }
}

void Mesh::InitMesh(const IndexedModel& model)
{
	m_numIndices = model.indices.size();

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	
	GLuint vboid;
	glGenBuffers(1, &vboid);
	m_VBO_ids.emplace_back(vboid);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &vboid);
	m_VBO_ids.emplace_back(vboid);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &vboid);
	m_VBO_ids.emplace_back(vboid);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &vboid);
	m_VBO_ids.emplace_back(vboid);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO_ids[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	for (int aa = m_VBO_ids.size() -1; aa >= 0; aa--)
	{
		glDeleteBuffers(1, &(m_VBO_ids[aa]));
	}
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::Render()
{
	glColor3f(static_cast<GLfloat> (1),	static_cast<GLfloat> (1),
		static_cast<GLfloat> (1));
	
	if (m_texture != nullptr) m_texture->Bind();
	
	glBindVertexArray(m_vertexArrayObject);

	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	//glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);
	//glDrawArrays(GL_TRIANGLES,0,3);

	glBindVertexArray(0);
	if (m_texture != nullptr)  m_texture->UnBind();
}

void Mesh::RenderInstanced(glm::vec3 *translations, int n_instances)
{
	if (!m_instancing_enabled)
	{
		InitializeInstancing(translations, n_instances);
	}
	glColor3f(static_cast<GLfloat> (1), static_cast<GLfloat> (1),
		static_cast<GLfloat> (1));

	if (m_texture != nullptr) m_texture->Bind();

	glBindVertexArray(m_vertexArrayObject);

	glDrawElementsInstanced(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, n_instances);
		
	//glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);
	//glDrawArrays(GL_TRIANGLES,0,3);

	glBindVertexArray(0);
	if (m_texture != nullptr)  m_texture->UnBind();
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
	return false;
}