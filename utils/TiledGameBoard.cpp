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

	m_tiles = new Tile[m_xtiles * m_ytiles];
	for (int aa = 0; aa < m_xtiles * m_ytiles; aa++)
	{
		m_tiles[aa].SetWindowSectionPtr(this);
	}
	
	m_scale_vec = glm::vec3(2.0 / m_xtiles, 2.0 / m_ytiles, 2.0 / m_xtiles);

	if (m_tiles == nullptr)
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
		}
	}

	if (m_gamemap_ptr->GetMainCharPtr() != nullptr)
	{
		AddObject(m_gamemap_ptr->GetMainCharPtr());
	}

}

void TiledGameBoard::Print()
{
	Character *mainchar = m_gamemap_ptr->GetMainCharPtr();
	for (int row = m_ytiles - 1; row >= 0; row--)
	{
		for (int col = 0; col < m_xtiles; col++)
		{
			Tile *curtile = &(m_tiles[GetTileIdx(col, row)]);
			int curtile_wx = mainchar->GetX() - (m_xtiles / 2) + col;
			int curtile_wy = mainchar->GetY() - (m_ytiles / 2) + row;
			TileType cur;
			m_gamemap_ptr->GetTileType(curtile_wx, curtile_wy, cur);
			std::cout << static_cast<int>(cur);
			if (col != m_xtiles - 1) std::cout << ",";
		}
		std::cout << std::endl;
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

	for (int row = 0; row < m_ytiles; row++)
	{
		for (int col = 0; col < m_xtiles; col++)
		{
			Tile *curtile = &(m_tiles[ GetTileIdx(col, row) ]);
			curtile->SetColor(RGB(255, 255, 255));
			TileType cur;
			int curtile_wx = maincharx - (m_xtiles / 2) + col;
			int curtile_wy = mainchary - (m_ytiles / 2) + row;
			m_gamemap_ptr->GetTileType(curtile_wx, curtile_wy, cur);
			curtile->SetTileType(cur);

			ids_per_mesh[cur].emplace_back(GetTileIdx(col, row));

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
		Tile *curtile = &(m_tiles[it1->second[0]]);
		Mesh *m1 = curtile->GetMeshPtr();
		int n_instances = it1->second.size();
		glm::mat4 *translations = new glm::mat4[n_instances];
		for (int aa = 0; aa < it1->second.size(); aa++)
		{
			Tile *tmp2 = &(m_tiles[it1->second[aa]]);
			translations[aa] =
				glm::translate(glm::mat4(1.0), glm::vec3(tmp2->GetXDrawPos_N11(), tmp2->GetYDrawPos_N11(), 0));
		}
		m1->SetUpInstancing(it1->second.size(), m_scale_vec, translations);
		delete[] translations;
		m1->Render();
	}

	WindowSection::Refresh();
}

int TiledGameBoard::GetTileIdx(int xpos, int ypos)
{
	return (xpos + (ypos * m_xtiles));
}
