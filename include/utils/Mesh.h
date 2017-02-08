#ifndef MESH_H__
#define MESH_H__

#include <glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "ObjLoader.h"
#include "DrawableObj.h"
#include "Texture.h"

enum class BufferIdx
{
	POS_IDX = 0,
	IDX_IDX = 1, // :)
	TEXTURE_IDX = 2,
	NORMAL_IDX = 3,
	TRANS_IDX = 4
};

enum class AttributeIdx
{
	POS_LOC = 0,
	TEXTURE_LOC = 1,
	NORMAL_LOC = 2,
	TRANSLATION_LOC = 3
};

// Mesh class is in this file below.
class Mesh;

class MeshManager
{

public:
	static MeshManager *GetInstance();
	bool GetMeshPtr(TileType type, Mesh **texture);
	bool GetMeshPtr(const std::string &filename, Mesh **texture);

private:
	std::map<std::string, Mesh *> m_lookup;
	static MeshManager *m_instance;
	static bool m_instance_created;
};

class Mesh : public DrawableObj
{
public:
	Mesh() {}
	virtual ~Mesh();
	friend class MeshManager;
	bool UseTexture(std::string &texture);
	bool UseTexture(TileType &ttype);
	void Render();
	void RenderInstanced();

protected:

private:
	Mesh(const std::string& fileName);
	Mesh(IndexedModel &model, bool calcNormalsForMe);
	void operator=(const Mesh& mesh) {}
	Mesh(const Mesh& mesh) {}

	void InitMesh(const IndexedModel& model);

	GLuint m_VAO;
	GLuint m_VBO_ids[5];
	IndexedModel m_model;
	unsigned int m_numIndices;
	Texture *m_texture;
	bool m_instancing_enabled;
};


#endif
