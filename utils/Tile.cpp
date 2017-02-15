#include "Tile.h"
#include "Texture.h"

TileType Tile::GetTileType()
{
	return m_tiletype;
}

void Tile::SetTileType(TileType type)
{
	if (type == m_tiletype && m_mesh_ptr != nullptr)
	{
		return;
	}

	m_tiletype = type;
	MeshManager *tm = MeshManager::GetInstance();
	tm->GetMeshPtr(type, &m_mesh_ptr);
	m_mesh_ptr->UseTexture(m_tiletype);
}

Mesh *Tile::GetMeshPtr()
{
	return m_mesh_ptr;
}

void Tile::Render()
{
	if (m_mesh_ptr != nullptr)
	{
		m_mesh_ptr->Render();
	}
}
