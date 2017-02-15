#ifndef TILE_H__
#define TILE_H__

#include "DrawableObj.h"
#include "Texture.h"
#include "RGB.h"
#include "Mesh.h"

class Tile : public Mesh
{
public:

	Tile()
	{
		m_mesh_ptr = nullptr;
		m_tiletype = TileType::GRASS;
	}

	void SetTileType(TileType type);
	TileType GetTileType();
	void Render();
	Mesh *GetMeshPtr();

private:

	TileType m_tiletype;
	Mesh *m_mesh_ptr;
};

#endif
