#include "Tile.h"
#include "Texture.h"

void Tile::SetColor(RGB col) { m_color = col; }

void Tile::SetTileType(TileType type)
{
	if (type == m_tiletype && m_texture != nullptr)
	{
		return;
	}

	TextureManager *tm = TextureManager::GetInstance();
	tm->GetTexturePtr(type, &m_texture);
	m_tiletype = type;
}

void Tile::Render()
{
	GLfloat tile_w( static_cast<GLfloat> (GetRelativeWidth_01() * 2 ));
	GLfloat tile_h( static_cast<GLfloat> (GetRelativeHeight_01() * 2 ));
	GLfloat x_pos, y_pos;

	glEnable(GL_TEXTURE_2D);
	m_texture->Bind();

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
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
