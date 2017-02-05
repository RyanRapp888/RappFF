#include "DrawnCharacter.h"

DrawnCharacter::DrawnCharacter()
{
	m_texture = nullptr;
	m_color = RGB(255, 255, 255);
}

void DrawnCharacter::SetTileType(TileType tt)
{
	if (tt == m_tiletype && m_texture != nullptr)
	{
		return;
	}

	TextureManager *tm = TextureManager::GetInstance();
	tm->GetTexturePtr(tt, &m_texture);
	m_tiletype = tt;
}

DrawnCharacter &DrawnCharacter::operator=(const DrawnCharacter & other)
{
	if (this != &other)
	{
		this->m_texture = other.m_texture;
		this->m_tiletype = other.m_tiletype;
		this->m_color = other.m_color;
     	DrawableObj::operator=(other);
	}
	return *this;
}

void DrawnCharacter::SetColor(RGB col) { m_color = col; }

void DrawnCharacter::Render()
{
	double tile_w(GetRelativeWidth_01() * 2);
	double tile_h(GetRelativeHeight_01() * 2);
	double x_pos, y_pos;

	m_texture->Bind();

	x_pos = GetXDrawPos_N11();
	y_pos = GetYDrawPos_N11();

	glColor4f(m_color.GetRed() / 255.0,
		m_color.GetGreen() / 255.0,
		m_color.GetBlue() / 255.0,
		0);

	glBegin(GL_TRIANGLES);
	glTexCoord2d(0.0, 0.0); glVertex3f(x_pos, y_pos, -.01);
	glTexCoord2d(1.0, 0.0); glVertex3f(x_pos + tile_w, y_pos, -0.01);
	glTexCoord2d(1.0, 1.0); glVertex3f(x_pos + tile_w, y_pos + tile_h, -0.01);
	glTexCoord2d(1.0, 1.0); glVertex3f(x_pos + tile_w, y_pos + tile_h, -0.01);
	glTexCoord2d(0.0, 1.0); glVertex3f(x_pos, y_pos + tile_h, -0.01);
	glTexCoord2d(0.0, 0.0); glVertex3f(x_pos, y_pos, -0.01);
	glEnd();

	m_texture->UnBind();
}
