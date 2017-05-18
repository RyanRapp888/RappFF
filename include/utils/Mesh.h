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

struct GLData
{
	GLuint m_vao;
	std::vector<GLuint> m_vbos;
	int m_n_vertices;
};

class MeshManager
{
   public:
	
   static MeshManager *GetInstance();
   bool GetGLData(const std::string &filename, GLData **gldata);

   private:
	
   std::map<std::string, GLData *> m_lookup;
   static MeshManager *m_instance;
   static bool m_initialized;
};

class Mesh : public DrawableObj
{
   public:

   friend class MeshManager;
   Mesh();
   bool LoadMesh(const std::string &meshname);
   virtual ~Mesh();
   bool UseTexture(std::string &texture);
   void SetUpInstancing(size_t n_instances, glm::vec3 scale, glm::mat4 *translations);
   void Render();
   
private:
   Mesh(const std::string& fileName);
   Mesh(IndexedModel &model);
   Mesh(const Mesh& mesh) {}
   void operator=(const Mesh& mesh) {}
     
   GLData *m_gldata;
   Texture *m_texture;
   size_t m_num_instances;
   RGB m_color;
};

#endif
