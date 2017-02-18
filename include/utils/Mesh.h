#ifndef MESH_H__
#define MESH_H__

#include <glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "ObjLoader.h"
#include "DrawableObj.h"
#include "TileType.h"
#include "Texture.h"
#include "RGB.h"

enum class BufferIdx
{
	POS_IDX = 0,
	TEXTURE_IDX = 1,
	NORMAL_IDX = 2,
	TRANS_IDX = 3
};

enum class AttributeIdx
{
	POS_LOC = 0,
	TEXTURE_LOC = 1,
	NORMAL_LOC = 2,
	TRANSLATION_LOC = 3
};

class Mesh;

class MeshManager
{
   public:
	
   static MeshManager *GetInstance();
   bool GetMeshPtr(const std::string &filename, Mesh **meshdat);

   private:
	
   std::map<std::string, Mesh *> m_lookup;
   static MeshManager *m_instance;
   static bool m_initialized;
};

class Mesh : public DrawableObj
{

public:

   friend class MeshManager;
   
   virtual ~Mesh();
   bool UseTexture(std::string &texture);
   void SetUpInstancing(int n_instances, glm::vec3 scale, glm::mat4 *translations);
   void Render();
   void InitMesh(const IndexedModel& model);
   void InitMesh(const std::string& filename);
protected:

   Mesh() 
   {
      m_VAO = 999;
      m_model.Clear();
      m_texture = nullptr;
      m_num_instances = 0;
      m_color = RGB(255, 255, 255);
	  m_is_valid = false;
   }

private:
   Mesh(const std::string& fileName);
   Mesh(IndexedModel &model);
   void operator=(const Mesh& mesh) {}
   Mesh(const Mesh& mesh) {}
   
   GLuint m_VAO;
   std::vector<GLuint> m_VBO_ids;
   IndexedModel m_model;
   Texture *m_texture;
   int m_num_instances;
   RGB m_color;
   bool m_is_valid;
};

#endif
