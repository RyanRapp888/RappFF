#include "Character.h"

void Character::SetLocation(int x, int y)
{
	m_worldx = x;
	m_worldy = y;
	if (m_worldx < 0) m_worldx = 0;
	if (m_worldx >= m_gamemap_ptr->GetWorldMaxX()) m_worldx = m_gamemap_ptr->GetWorldMaxX() - 1;
	if (m_worldy < 0) m_worldy = 0;
	if (m_worldy >= m_gamemap_ptr->GetWorldMaxY()) m_worldy = m_gamemap_ptr->GetWorldMaxY() - 1;
}

int Character::GetWorldX() { return m_worldx; }

int Character::GetWorldY() { return m_worldy; }
