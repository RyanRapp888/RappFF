#include "Tile.h"

static std::map<TileType, std::string> tiletype_to_mesh
{
	{ TileType::BLACK, "assets\\models\\flattile.robj" },
	{ TileType::BRICKS, "assets\\models\\flattile.robj" },
	{ TileType::DESERT, "assets\\models\\flattile.robj" },
	{ TileType::EMMY, "assets\\models\\cube.robj" },
	{ TileType::FAIRY, "assets\\models\\cube.robj" },
	{ TileType::GRASS, "assets\\models\\flattile.robj" },
	{ TileType::GRASS2, "assets\\models\\flattile.robj" },
	{ TileType::JELLYBEAN, "assets\\models\\cube.robj" },
	{ TileType::MAINCHAR, "assets\\models\\cube.robj" },
	{ TileType::MERMAID, "assets\\models\\cube.robj" },
	{ TileType::MTN, "assets\\models\\mtnshape.robj" },
	{ TileType::MTNSNOW, "assets\\models\\mtnshape.robj" },
	{ TileType::MUD, "assets\\models\\flattile.robj" },
	{ TileType::OCTOPUS, "assets\\models\\cube.robj" },
	{ TileType::PARCHMENT, "assets\\models\\flattile.robj" },
	{ TileType::PLANK, "assets\\models\\flattile.robj" },
	{ TileType::PRINCESS, "assets\\models\\cube.robj" },
	{ TileType::ROCKS, "assets\\models\\mtnshape.robj" },
	{ TileType::ROOF, "assets\\models\\flattile.robj" },
	{ TileType::WATER, "assets\\models\\flattile.robj" },
	{ TileType::WOOD, "assets\\models\\flattile.robj" },
	{ TileType::FRAME, "assets\\models\\flattile.robj"},
	{ TileType::SKELETON, "assets\\models\\cube.robj" },
	{ TileType::BEAR, "assets\\models\\cube.robj" } 
};

static std::map<TileType, std::string> tiletype_to_texture
{
	{ TileType::BLACK, "assets\\textures\\black.jpg" },
	{ TileType::BRICKS, "assets\\textures\\bricks.jpg" },
	{ TileType::DESERT, "assets\\textures\\desert.jpg" },
	{ TileType::EMMY, "assets\\textures\\emmy.png" },
	{ TileType::FAIRY, "assets\\textures\\fairy.png" },
	{ TileType::GRASS, "assets\\textures\\grass.png" },
	{ TileType::GRASS2, "assets\\textures\\grass2.png" },
	{ TileType::JELLYBEAN, "assets\\textures\\jellybean.jpg" },
	{ TileType::MAINCHAR, "assets\\textures\\cat.png" },
	{ TileType::MERMAID, "assets\\textures\\mermaid.jpg" },
	{ TileType::MTN, "assets\\textures\\mtn.png" },
	{ TileType::MTNSNOW, "assets\\textures\\mtnsnow.png" },
	{ TileType::MUD, "assets\\textures\\mud.png" },
	{ TileType::OCTOPUS, "assets\\textures\\octopus.png" },
	{ TileType::PARCHMENT, "assets\\textures\\parchment.jpg" },
	{ TileType::PLANK, "assets\\textures\\Plank.png" },
	{ TileType::PRINCESS, "assets\\textures\\princess.png" },
	{ TileType::ROCKS, "assets\\textures\\rocks.jpg" },
	{ TileType::ROOF, "assets\\textures\\roof.png" },
	{ TileType::WATER, "assets\\textures\\Water.png" },
	{ TileType::WOOD, "assets\\textures\\Wood.png" },
	{ TileType::FRAME, "assets\\textures\\framewhite.jpg"},
	{ TileType::SKELETON, "assets\\textures\\skeleton.jpg"},
	{ TileType::BEAR, "assets\\textures\\bear.jpg"}
};

std::string GetMeshFilename(TileType tt)
{
	return tiletype_to_mesh[tt];
}

std::string GetTextureFilename(TileType tt)
{
	return tiletype_to_texture[tt];
}

TileType Tile::GetTileType()
{
	return m_tiletype;
}

void Tile::SetTileType(TileType type)
{
	if (type == m_tiletype)
	{
		return;
	}

	auto iter = tiletype_to_mesh.find(type);
	if (iter != tiletype_to_mesh.end())
	{
		Mesh::LoadMesh(iter->second);
	}

	auto iter2 = tiletype_to_texture.find(type);
	if (iter2 != tiletype_to_texture.end())
	{
		Mesh::UseTexture(iter2->second);
	}
	m_tiletype = type;
	return;
}

void Tile::Render()
{
	Mesh::Render();
}
