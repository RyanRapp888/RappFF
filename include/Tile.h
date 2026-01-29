#ifndef TILE_H__
#define TILE_H__

#include "TileType.h"
#include "Mesh.h"

std::string GetMeshFilename(TileType tt);
std::string GetTextureFilename(TileType tt);

class Tile: public Mesh
{
public:

	Tile() : Mesh()
	{
    	m_tiletype = TileType::UNDEF;
	}

	void SetTileType(TileType type);
	TileType GetTileType();
	void Render();
	
private:

	TileType m_tiletype;
};

#endif
