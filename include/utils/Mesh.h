#ifndef MESH_H__
#define MESH_H__

#include <glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "ObjLoader.h"
#include "DrawableObj.h"
#include "Texture.h"

enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	INDEX_VB
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
	friend class MeshManager;
	bool UseTexture(std::string &texture);
	bool UseTexture(TileType &ttype);
	void Render();
	virtual ~Mesh();

protected:

private:
	Mesh(const std::string& fileName);
	Mesh(IndexedModel &model, bool calcNormalsForMe);
	static const unsigned int NUM_BUFFERS = 4;
	void operator=(const Mesh& mesh) {}
	Mesh(const Mesh& mesh) {}

	void InitMesh(const IndexedModel& model);

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_numIndices;
	Texture *m_texture;
};


#endif
