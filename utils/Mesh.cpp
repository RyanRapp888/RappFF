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

static void GenerateVAOVBOS(IndexedModel &model, GLData **gldata)
{
	*gldata = new GLData;

	(*gldata)->m_n_vertices = static_cast<int>(model.m_positions.size());
	glGenVertexArrays(1, &((*gldata)->m_vao));
	glBindVertexArray((*gldata)->m_vao);

	GLuint tmp;
	glGenBuffers(1, &tmp);
	(*gldata)->m_vbos.push_back(tmp);
	glGenBuffers(1, &tmp);
	(*gldata)->m_vbos.push_back(tmp);
	glGenBuffers(1, &tmp);
	(*gldata)->m_vbos.push_back(tmp);

	// Spatial Positions (-1 to 1)
	glBindBuffer(GL_ARRAY_BUFFER, (*gldata)->m_vbos[static_cast<int>(BufferIdx::POS_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.m_positions[0]) * model.m_positions.size(), &model.m_positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(static_cast<int>(AttributeIdx::POS_LOC));
	glVertexAttribPointer(static_cast<int>(AttributeIdx::POS_LOC), 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Texture Coords
	glBindBuffer(GL_ARRAY_BUFFER, (*gldata)->m_vbos[static_cast<int>(BufferIdx::TEXTURE_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.m_texCoords[0]) * model.m_texCoords.size(), &model.m_texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(static_cast<int>(AttributeIdx::TEXTURE_LOC));
	glVertexAttribPointer(static_cast<int>(AttributeIdx::TEXTURE_LOC), 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, (*gldata)->m_vbos[static_cast<int>(BufferIdx::NORMAL_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.m_normals[0]) * model.m_normals.size(), &model.m_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(static_cast<int>(AttributeIdx::NORMAL_LOC));
	glVertexAttribPointer(static_cast<int>(AttributeIdx::NORMAL_LOC), 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

bool MeshManager::GetGLData(const std::string &filename, GLData **gldata)
{
	auto iter2 = m_lookup.find(filename);
	if (iter2 != m_lookup.end())
	{
		*gldata = iter2->second;
		return true;
	}
	
	IndexedModel model;
	model = OBJModel(filename).ToIndexedModel();
	GenerateVAOVBOS(model, gldata);
	m_lookup[filename] = *gldata;
	return true;
}

Mesh::Mesh()
{
	m_texture = nullptr;
	m_gldata = nullptr;
	m_num_instances = 0;
	m_color = RGB(255, 255, 255);
}

Mesh::Mesh(const std::string& fileName)
{
	LoadMesh(fileName);
}

bool Mesh::LoadMesh(const std::string &filename)
{
	MeshManager *mm = MeshManager::GetInstance();
	mm->GetGLData(filename, &m_gldata);
	return m_gldata != nullptr;
}

bool Mesh::UseTexture(std::string &texture_filename)
{
   if (texture_filename.size() == 0) return false;
   TextureManager *tm = TextureManager::GetInstance();
   tm->GetTexturePtr(texture_filename, &m_texture);
   if (m_texture != nullptr) return true;
   return false;
}

void Mesh::SetUpInstancing(size_t n_instances, glm::vec3 scalevect, glm::mat4 *translations)
{
	glBindVertexArray(m_gldata->m_vao);
	if (m_gldata->m_vbos.size() < 4)
	{
		GLuint tmp;
		glGenBuffers(1, &tmp);
		m_gldata->m_vbos.push_back(tmp);
	}

   m_num_instances = n_instances;
   
   for (int aa = 0; aa < n_instances; aa++)
   {
	   translations[aa] = glm::scale(translations[aa], scalevect);
   }

   glBindBuffer(GL_ARRAY_BUFFER, m_gldata->m_vbos[static_cast<int>(BufferIdx::TRANS_IDX)]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*n_instances, translations, GL_STATIC_DRAW);

	for (int i = 0; i < 4; ++i)
	{
		glVertexAttribPointer(static_cast<int>(AttributeIdx::TRANSLATION_LOC) + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid *)(i * sizeof(glm::vec4)));
		glVertexAttribDivisor(static_cast<int>(AttributeIdx::TRANSLATION_LOC) + i, 1);
		glEnableVertexAttribArray(static_cast<int>(AttributeIdx::TRANSLATION_LOC) + i);
	}
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	
}

void Mesh::Render()
{
   glBindVertexArray(m_gldata->m_vao);
   glColor3f(1.0, 1.0, 1.0);
  
   if (m_texture != nullptr) m_texture->Bind();

   if (m_num_instances > 0)
   {
	   glDrawArraysInstanced(GL_TRIANGLES, 0, m_gldata->m_n_vertices, static_cast<GLsizei> (m_num_instances));
	   glFlush();
   }
   
   if (m_texture != nullptr) m_texture->UnBind();
   glBindVertexArray(0);
}
