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
	InitMesh(fileName);
}

Mesh::Mesh(IndexedModel &model)
{
   InitMesh(model);
   m_texture = nullptr;
   m_num_instances = 0;
   m_color = RGB(255, 255, 255);
   m_is_valid = true;
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
	if (m_VBO_ids.size() < 4)
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

void Mesh::InitMesh(const std::string &filename)
{
	if (GetPrefabModel(filename, m_model))
	{
		InitMesh(m_model);
	}
	else
	{
		m_model = OBJModel(filename).ToIndexedModel();
		InitMesh(m_model);
	}
	m_texture = nullptr;
	m_num_instances = 0;
	m_color = RGB(255, 255, 255);
	m_is_valid = true;
}

void Mesh::InitMesh(const IndexedModel& model)
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	
	GLuint tmp;
	glGenBuffers(1, &tmp);
	m_VBO_ids.push_back(tmp);
	glGenBuffers(1, &tmp);
	m_VBO_ids.push_back(tmp);
	glGenBuffers(1, &tmp);
	m_VBO_ids.push_back(tmp);
		
	// Spatial Positions (-1 to 1)
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::POS_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.m_positions[0]) * model.m_positions.size(), &model.m_positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(static_cast<int>(AttributeIdx::POS_LOC));
	glVertexAttribPointer(static_cast<int>(AttributeIdx::POS_LOC), 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Texture Coords
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::TEXTURE_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.m_texCoords[0]) * model.m_texCoords.size(), &model.m_texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(static_cast<int>(AttributeIdx::TEXTURE_LOC));
	glVertexAttribPointer(static_cast<int>(AttributeIdx::TEXTURE_LOC), 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ids[static_cast<int>(BufferIdx::NORMAL_IDX)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.m_normals[0]) * model.m_normals.size(), &model.m_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(static_cast<int>(AttributeIdx::NORMAL_LOC));
	glVertexAttribPointer(static_cast<int>(AttributeIdx::NORMAL_LOC), 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	m_is_valid = true;
}

Mesh::~Mesh()
{
	
}

void Mesh::Render()
{
   if (!m_is_valid) return;
   glBindVertexArray(m_VAO);
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   //glEnable(GL_BLEND);
   glColor3f(1.0, 1.0, 1.0);
  
   if (m_texture != nullptr) m_texture->Bind();

   if (m_num_instances > 0)
   {
	   glDrawArraysInstanced(GL_TRIANGLES, 0, m_model.m_positions.size(), m_num_instances);
	   glFlush();
   }
   else
   {
       glDrawArrays(GL_TRIANGLES, 0, m_model.m_positions.size());
   }
   
   if (m_texture != nullptr) m_texture->UnBind();
   glBindVertexArray(0);
}

bool GetPrefabModel(const std::string &str, IndexedModel &dat)
{
	if (str == "res\\mtnshape.robj")
	{
		
		dat.Clear();
		dat.m_positions.emplace_back(glm::vec3(-.5f, -.5, 0)); //LL
		dat.m_positions.emplace_back(glm::vec3(.5f, -.5, 0)); //LR
		dat.m_positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE

		dat.m_positions.emplace_back(glm::vec3(.5f, -.5, 0)); //LR
		dat.m_positions.emplace_back(glm::vec3(.5f, .5, 0)); //UR
		dat.m_positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE

		dat.m_positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE
		dat.m_positions.emplace_back(glm::vec3(.5f, .5, 0)); //UR
		dat.m_positions.emplace_back(glm::vec3(-.5f, .5, 0)); //UL

		dat.m_positions.emplace_back(glm::vec3(-.5f, -.5, 0)); //LL
		dat.m_positions.emplace_back(glm::vec3(0, 0, -.5)); //CENTRE
		dat.m_positions.emplace_back(glm::vec3(-.5f, .5, 0)); //UL

		dat.m_texCoords.emplace_back(0, 0); //LL
		dat.m_texCoords.emplace_back(1, 0); //LR
		dat.m_texCoords.emplace_back(0, 0); //C

		dat.m_texCoords.emplace_back(1, 0); //LR
		dat.m_texCoords.emplace_back(1, 1); //UR
		dat.m_texCoords.emplace_back(0, 0); //C

		dat.m_texCoords.emplace_back(0, 0); //C
		dat.m_texCoords.emplace_back(1, 1); //UR
		dat.m_texCoords.emplace_back(0, 1); //UL

		dat.m_texCoords.emplace_back(0, 0); //LL
		dat.m_texCoords.emplace_back(0, 0); //C
		dat.m_texCoords.emplace_back(0, 1); //UL

		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		return true;
	}
	else if ( str == "res\\emmy.robj" || str == "res\\fairy.robj" || str == "res\\jellybean.robj" ||
		      str == "res\\mermaid.robj" || str == "res\\octopus.robj" || str == "res\\princess.robj")
	{ 
		dat.Clear();

		//top of cube
		dat.m_positions.emplace_back(glm::vec3(-.25f, -.25, -1.0)); //LL
		dat.m_positions.emplace_back(glm::vec3(.25f, -.25, -1.0)); //LR
		dat.m_positions.emplace_back(glm::vec3(.25f, .25, -1.0)); //UR
		dat.m_positions.emplace_back(glm::vec3(-.25f, -.25, -1.0)); //LL
		dat.m_positions.emplace_back(glm::vec3(.25f, .25, -1.0)); //UR
		dat.m_positions.emplace_back(glm::vec3(-.25f, .25, -1.0)); //UL
		
		//bottom of cube
		dat.m_positions.emplace_back(glm::vec3(-.25f, -.25, -.5)); //LL2
		dat.m_positions.emplace_back(glm::vec3(.25f, -.25, -.5)); //LR2
		dat.m_positions.emplace_back(glm::vec3(.25f, .25, -.5)); //UR2
		dat.m_positions.emplace_back(glm::vec3(-.25f, -.25, -.5)); //LL2
		dat.m_positions.emplace_back(glm::vec3(.25f, .25, -0.5)); //UR2
		dat.m_positions.emplace_back(glm::vec3(-.25f, .25, -.5)); //UL2

	    //side of cube
		dat.m_positions.emplace_back(glm::vec3(-.25f, -.25, -.5)); //LL2
		dat.m_positions.emplace_back(glm::vec3(.25f, -.25, -.5)); //LR2
		dat.m_positions.emplace_back(glm::vec3(.25f, -.25, -1.0)); //LR
		dat.m_positions.emplace_back(glm::vec3(-.25f, -.25, -.5)); //LL2
		dat.m_positions.emplace_back(glm::vec3(.25f, -.25, -1.0)); //LR
		dat.m_positions.emplace_back(glm::vec3(-.25f, -.25, -1.0)); //LL

		//side of cube
		dat.m_positions.emplace_back(glm::vec3(.25f, -.25, -.5)); //LR2
		dat.m_positions.emplace_back(glm::vec3(.25f, .25, -0.5)); //UR2
		dat.m_positions.emplace_back(glm::vec3(.25f, .25, -1.0)); //UR
		dat.m_positions.emplace_back(glm::vec3(.25f, -.25, -.5)); //LR2
		dat.m_positions.emplace_back(glm::vec3(.25f, .25, -1.0)); //UR
		dat.m_positions.emplace_back(glm::vec3(.25f, -.25, -1.0)); //LR

		// side of cube
		dat.m_positions.emplace_back(glm::vec3(-.25f, .25, -.5)); //UL2
		dat.m_positions.emplace_back(glm::vec3(.25f, .25, -0.5)); //UR2
		dat.m_positions.emplace_back(glm::vec3(.25f, .25, -1.0)); //UR
		dat.m_positions.emplace_back(glm::vec3(-.25f, .25, -.5)); //UL2
		dat.m_positions.emplace_back(glm::vec3(.25f, .25, -1.0)); //UR
		dat.m_positions.emplace_back(glm::vec3(-.25f, .25, -1.0)); //UL

		// side of cube
		dat.m_positions.emplace_back(glm::vec3(-.25f, -.25, -.5)); //LL2
		dat.m_positions.emplace_back(glm::vec3(-.25f, .25, -.5)); //UL2
		dat.m_positions.emplace_back(glm::vec3(-.25f, .25, -1.0)); //UL
		dat.m_positions.emplace_back(glm::vec3(-.25f, -.25, -.5)); //LL2
		dat.m_positions.emplace_back(glm::vec3(-.25f, .25, -1.0)); //UL
		dat.m_positions.emplace_back(glm::vec3(-.25f, -.25, -1.0)); //LL

		// texcoords top
		dat.m_texCoords.emplace_back(0, 0); //LL
		dat.m_texCoords.emplace_back(1, 0); //LR
		dat.m_texCoords.emplace_back(1, 1); //UR
		dat.m_texCoords.emplace_back(0, 0); //LL
		dat.m_texCoords.emplace_back(1, 1); //UR
		dat.m_texCoords.emplace_back(0, 1); //UL

		//texcoords bottom
		dat.m_texCoords.emplace_back(0, 0); //LL
		dat.m_texCoords.emplace_back(1, 0); //LR
		dat.m_texCoords.emplace_back(1, 1); //UR
		dat.m_texCoords.emplace_back(0, 0); //LL
		dat.m_texCoords.emplace_back(1, 1); //UR
		dat.m_texCoords.emplace_back(0, 1); //UL
		
		//side of cube
		dat.m_texCoords.emplace_back(0,0); 
		dat.m_texCoords.emplace_back(1,0);
		dat.m_texCoords.emplace_back(1,1); 
		dat.m_texCoords.emplace_back(0,0); 
		dat.m_texCoords.emplace_back(1,1); 
		dat.m_texCoords.emplace_back(0,1); 

		//side of cube
		dat.m_texCoords.emplace_back(0, 0);
		dat.m_texCoords.emplace_back(1, 0); 
		dat.m_texCoords.emplace_back(1, 1); 
		dat.m_texCoords.emplace_back(0, 0); 
		dat.m_texCoords.emplace_back(1, 1); 
		dat.m_texCoords.emplace_back(0, 1); 

		// side of cube
		dat.m_texCoords.emplace_back(0, 0); 
		dat.m_texCoords.emplace_back(1, 0);
		dat.m_texCoords.emplace_back(1, 1); 
		dat.m_texCoords.emplace_back(0, 0); 
		dat.m_texCoords.emplace_back(1, 1); 
		dat.m_texCoords.emplace_back(0, 1); 
		
	    // side of cube
		dat.m_texCoords.emplace_back(0, 0);
		dat.m_texCoords.emplace_back(1, 0); 
		dat.m_texCoords.emplace_back(1, 1);
		dat.m_texCoords.emplace_back(0, 0);
		dat.m_texCoords.emplace_back(1, 1);
		dat.m_texCoords.emplace_back(0, 1);

		dat.m_normals.resize(dat.m_positions.size());
		for (int aa = 0; aa < dat.m_normals.size(); aa++)
		{
			dat.m_normals[aa] = glm::vec3(0, 0, -1);
		}
		return true;
	}
	else if(str == "res\\flattile.robj")
	{
		dat.Clear();
		dat.m_positions.emplace_back(glm::vec3(-.5f, -.5, 0)); //LL
		dat.m_positions.emplace_back(glm::vec3(.5f, -.5, 0)); //LR
		dat.m_positions.emplace_back(glm::vec3(.5f, .5, 0)); //UR

		dat.m_positions.emplace_back(glm::vec3(-.5f, -.5, 0)); //LL
		dat.m_positions.emplace_back(glm::vec3(.5f, .5, 0)); //UR
		dat.m_positions.emplace_back(glm::vec3(-.5f, .5, 0)); //UL
     
		dat.m_texCoords.emplace_back(0, 0); //LL
		dat.m_texCoords.emplace_back(1, 0); //LR
		dat.m_texCoords.emplace_back(1, 1); //UR

		dat.m_texCoords.emplace_back(0, 0); //LL
		dat.m_texCoords.emplace_back(1, 1); //UR
		dat.m_texCoords.emplace_back(0, 1); //UL
		
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		dat.m_normals.emplace_back(glm::vec3(0, 0, -1));
		return true;
	}
	return false;
}
