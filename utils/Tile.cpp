#include "Tile.h"

static std::map<TileType, std::string> tiletype_to_mesh
{
	{ TileType::BLACK, "res\\flattile.robj" },
	{ TileType::BRICKS, "res\\flattile.robj" },
	{ TileType::DESERT, "res\\flattile.robj" },
	{ TileType::EMMY, "res\\cube.robj" },
	{ TileType::FAIRY, "res\\cube.robj" },
	{ TileType::GRASS, "res\\flattile.robj" },
	{ TileType::GRASS2, "res\\flattile.robj" },
	{ TileType::JELLYBEAN, "res\\cube.robj" },
	{ TileType::MAINCHAR, "res\\cube.robj" },
	{ TileType::MERMAID, "res\\cube.robj" },
	{ TileType::MTN, "res\\mtnshape.robj" },
	{ TileType::MTNSNOW, "res\\mtnshape.robj" },
	{ TileType::MUD, "res\\flattile.robj" },
	{ TileType::OCTOPUS, "res\\cube.robj" },
	{ TileType::PARCHMENT, "res\\flattile.robj" },
	{ TileType::PLANK, "res\\flattile.robj" },
	{ TileType::PRINCESS, "res\\cube.robj" },
	{ TileType::ROCKS, "res\\mtnshape.robj" },
	{ TileType::ROOF, "res\\flattile.robj" },
	{ TileType::WATER, "res\\flattile.robj" },
	{ TileType::WOOD, "res\\flattile.robj" },
	{ TileType::FRAME, "res\\flattile.robj"}
};

static std::map<TileType, std::string> tiletype_to_texture
{
	{ TileType::BLACK, "res\\black.jpg" },
	{ TileType::BRICKS, "res\\bricks.jpg" },
	{ TileType::DESERT, "res\\desert.jpg" },
	{ TileType::EMMY, "res\\emmy.png" },
	{ TileType::FAIRY, "res\\fairy.png" },
	{ TileType::GRASS, "res\\grass.png" },
	{ TileType::GRASS2, "res\\grass2.png" },
	{ TileType::JELLYBEAN, "res\\jellybean.jpg" },
	{ TileType::MAINCHAR, "res\\cat.png" },
	{ TileType::MERMAID, "res\\mermaid.jpg" },
	{ TileType::MTN, "res\\mtn.png" },
	{ TileType::MTNSNOW, "res\\mtnsnow.png" },
	{ TileType::MUD, "res\\mud.png" },
	{ TileType::OCTOPUS, "res\\octopus.png" },
	{ TileType::PARCHMENT, "res\\parchment.jpg" },
	{ TileType::PLANK, "res\\Plank.png" },
	{ TileType::PRINCESS, "res\\princess.png" },
	{ TileType::ROCKS, "res\\rocks.jpg" },
	{ TileType::ROOF, "res\\roof.png" },
	{ TileType::WATER, "res\\Water.png" },
	{ TileType::WOOD, "res\\Wood.png" },
	{ TileType::FRAME, "res\\framewhite.jpg"}
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
