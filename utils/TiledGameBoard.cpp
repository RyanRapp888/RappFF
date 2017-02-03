#include "TiledGameBoard.h"

void TiledGameBoard::AttachMainCharacter(Character *mc)
{
	m_mainchar_ptr = mc;
	AddObject(mc);
}

void TiledGameBoard::SetGameMapPtr(GameMap *gptr)
{
	m_gamemap_ptr = gptr;
}

void TiledGameBoard::SetTileDetails(int xtiles, int ytiles)
{
	m_xtiles = xtiles;
	m_ytiles = ytiles;

	m_tiles = new Tile[m_xtiles * m_ytiles];

	for (int xx = 0; xx < m_xtiles; xx++)
	{
		for (int yy = 0; yy < m_ytiles; yy++)
		{
			Tile *curtile = &(m_tiles[GetTileIdx(xx, yy)]);
			curtile->SetColor(RGB(255, 255, 255));
			TileType cur;
			m_gamemap_ptr->GetTileType(xx, yy, cur);
			curtile->SetTileType(cur);
			curtile->SetRelativeLocation(xx * (1.0 / m_xtiles), yy * (1.0 / m_ytiles), 1.0 / m_xtiles, 1.0 / m_ytiles);
			AddObject(curtile);
		}
	}

}

void TiledGameBoard::Refresh()
{
	for (int xx = 0; xx < m_xtiles; xx++)
	{
		for (int yy = 0; yy < m_ytiles; yy++)
		{
			Tile *curtile = &(m_tiles[GetTileIdx(xx, yy)]);
			curtile->SetColor(RGB(255, 255, 255));
			TileType cur;
			int curtile_wx = m_mainchar_ptr->GetWorldX() - (m_xtiles / 2) + xx;
			int curtile_wy = m_mainchar_ptr->GetWorldY() - (m_ytiles / 2) + yy;
			m_gamemap_ptr->GetTileType(curtile_wx, curtile_wy, cur);
			curtile->SetTileType(cur);
			for (int aa = 0; aa < m_otherchar_ptrs.size(); aa++)
			{
				Character *cur = m_otherchar_ptrs[aa];
				int col = cur->GetWorldX();
				int row = cur->GetWorldY();
				if (curtile_wx == col && curtile_wy == row)
				{
					double ox, oy, w, h;
					curtile->GetRelativeLocation(ox, oy, w, h);
					cur->SetRelativeLocation(ox, oy, w, h);
					cur->Render();
				}
			}
		}
	}

	int screen_mid_tilex(m_xtiles / 2);
	int screen_mid_tiley(m_ytiles / 2);
	Tile *center_tile = &(m_tiles[GetTileIdx(screen_mid_tilex, screen_mid_tiley)]);
	double ox, oy, w, h;
	center_tile->GetRelativeLocation(ox, oy, w, h);
	m_mainchar_ptr->SetRelativeLocation(ox, oy, w, h);
	WindowSection::Refresh();
}

int TiledGameBoard::GetTileIdx(int xpos, int ypos)
{
	return (xpos + (ypos * m_xtiles));
}
