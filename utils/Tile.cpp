#include "Tile.h"
#include "Texture.h"

void Tile::SetColor(RGB col) { m_color = col; }

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
	GLfloat tile_w( static_cast<GLfloat> (GetRelativeWidth_01() * 2 ));
	GLfloat tile_h( static_cast<GLfloat> (GetRelativeHeight_01() * 2 ));
	GLfloat x_pos, y_pos;

	if (m_mesh_ptr != nullptr)
	{
		m_mesh_ptr->Render();
	}
/*
	x_pos = static_cast<GLfloat> (GetXDrawPos_N11());
	y_pos = static_cast<GLfloat> (GetYDrawPos_N11());

	glColor3f( static_cast<GLfloat> (m_color.GetRed() / 255.0),
		       static_cast<GLfloat> (m_color.GetGreen() / 255.0),
		       static_cast<GLfloat> (m_color.GetBlue() / 255.0 ) );

	glBegin(GL_TRIANGLES);
	glTexCoord2d(0.0, 0.0); glVertex3f(x_pos, y_pos, 0);
	glTexCoord2d(1.0, 0.0); glVertex3f(x_pos + tile_w, y_pos, 0);
	glTexCoord2d(1.0, 1.0); glVertex3f(x_pos + tile_w, y_pos + tile_h, 0);
	glTexCoord2d(1.0, 1.0); glVertex3f(x_pos + tile_w, y_pos + tile_h, 0);
	glTexCoord2d(0.0, 1.0); glVertex3f(x_pos, y_pos + tile_h, 0);
	glTexCoord2d(0.0, 0.0); glVertex3f(x_pos, y_pos, 0);
	glEnd();
	*/
	//m_mesh_ptr->UnBind();
}
