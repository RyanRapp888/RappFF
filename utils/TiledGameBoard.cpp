#include "TiledGameBoard.h"

void TiledGameBoard::SetGameMapPtr(GameMap *gptr)
{
	m_gamemap_ptr = gptr;
}

void TiledGameBoard::SetTileDetails(int xtiles, int ytiles)
{
	m_xtiles = xtiles;
	m_ytiles = ytiles;

	if (m_tiles != nullptr) delete[] m_tiles;
	m_tiles = nullptr;

	if (m_translations != nullptr) delete[] m_translations;
	m_translations = nullptr;

	m_tiles = new Tile[m_xtiles * m_ytiles];
	m_translations = new glm::vec3[m_xtiles * m_ytiles];
		
	if (m_tiles == nullptr || m_translations == nullptr)
	{
		std::cout << "Error: could not allocate tiles" << std::endl;
	}

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
			glm::vec3 translation;
			translation.x = (GLfloat)xx * (1.0 / m_xtiles);
			translation.y = (GLfloat)yy * (1.0 / m_ytiles);
			translation.z = 0;
			glm::vec3 *cur_transloc = &(m_translations[GetTileIdx(xx, yy)]);
			cur_transloc->x = (translation.x * 2.0) - 1;
			cur_transloc->y = (translation.y * 2.0) - 1;
			cur_transloc->z = (translation.z * 2.0) - 1;
			//AddObject(curtile);
		}
	}

	if (m_gamemap_ptr->GetMainCharPtr() != nullptr)
	{
		AddObject(m_gamemap_ptr->GetMainCharPtr());
	}

}

void TiledGameBoard::Refresh()
{
	if (m_gamemap_ptr->GetMainCharPtr() == nullptr)
	{
		std::cout << "Error: GameMap Ptr not set" << std::endl;
		return;
	}

	int maincharx, mainchary;
	Character *mainchar = m_gamemap_ptr->GetMainCharPtr();
	maincharx = mainchar->GetX();
	mainchary = mainchar->GetY();
	std::vector< Character *> close_chars;
	m_gamemap_ptr->FindCharactersInRange
	   (maincharx - (m_xtiles / 2), mainchary - (m_ytiles / 2),
		maincharx + (m_xtiles / 2), mainchary + (m_ytiles / 2),
		close_chars);
	
	std::map<TileType, std::vector< int > > ids_per_mesh;

	for (int xx = 0; xx < m_xtiles; xx++)
	{
		for (int yy = 0; yy < m_ytiles; yy++)
		{
			Tile *curtile = &(m_tiles[GetTileIdx(xx, yy)]);
			curtile->SetColor(RGB(255, 255, 255));
			TileType cur;
			int curtile_wx = maincharx - (m_xtiles / 2) + xx;
			int curtile_wy = mainchary - (m_ytiles / 2) + yy;
			m_gamemap_ptr->GetTileType(curtile_wx, curtile_wy, cur);
			
			ids_per_mesh[cur].emplace_back(GetTileIdx(xx, yy));

			curtile->SetTileType(cur);
			for (int aa = 0; aa < close_chars.size(); aa++)
			{
				Character *cur = close_chars[aa];
				cur->SetWindowSectionPtr(this);
				int col = cur->GetX();
				int row = cur->GetY();
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
	mainchar->SetRelativeLocation(ox, oy, w, h);
	
	std::map<TileType, std::vector< int > >::iterator it1;
	for (it1 = ids_per_mesh.begin(); it1 != ids_per_mesh.end(); it1++)
	{
		for (int aa = 0; aa < it1->second.size(); aa++)
		{
			Tile *curtile = &(m_tiles[it1->second[aa]]);

		}
	}


	//WindowSection::Refresh();
}

int TiledGameBoard::GetTileIdx(int xpos, int ypos)
{
	return (xpos + (ypos * m_xtiles));
}
