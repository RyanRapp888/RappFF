#include "TiledGameBoard.h"

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
	
	m_scale_vec = glm::vec3(2.0 / m_xtiles,2.0 / m_ytiles, 2.0 / m_xtiles);

	if (m_tiles == nullptr)
	{
		std::cout << "Error: could not allocate tiles" << std::endl;
	}

	GameMap *map_ptr = GameMap::GetInstance();

	for (int xx = 0; xx < m_xtiles; xx++)
	{
		for (int yy = 0; yy < m_ytiles; yy++)
		{
			Tile *curtile = &(m_tiles[GetTileIdx(xx, yy)]);
			TileType cur;
			map_ptr->GetTileType(xx, yy, cur);
			curtile->SetTileType(cur);
			//curtile->SetRelativeLocation(xx * (2.0 / m_xtiles), yy * (2.0 / m_ytiles), 2.0 / m_xtiles, 2.0 / m_ytiles);
			curtile->SetRelativeLocation(xx * (1.0 / m_xtiles), yy * (1.0 / m_ytiles), 1.0 / m_xtiles, -.01);
		}
	}
}

void TiledGameBoard::Refresh()
{
	if (!m_enable) return;
	GameMap *map_ptr = GameMap::GetInstance();
	if (map_ptr->GetMainCharPtr() == nullptr)
	{
		std::cout << "Error: GameMap Ptr not set" << std::endl;
		return;
	}

	Character *mainchar = map_ptr->GetMainCharPtr();
	MeshManager *mesh_manager = MeshManager::GetInstance();
		
    std::vector< Character *> close_chars;
	map_ptr->FindCharactersInRange(mainchar->GetX() - (m_xtiles / 2), 
		                           mainchar->GetY() - (m_ytiles / 2),
		                           mainchar->GetX() + (m_xtiles / 2), 
		                           mainchar->GetY() + (m_ytiles / 2),
		                           close_chars);

	std::map<TileType, std::vector< int > > ids_per_tiletype;

	for (int row = 0; row < m_ytiles; row++)
	{
		for (int col = 0; col < m_xtiles; col++)
		{
			Tile *curtile = &(m_tiles[ GetTileIdx(col, row) ]);
			int curtile_wx = mainchar->GetX() - (m_xtiles / 2) + col;
			int curtile_wy = mainchar->GetY() - (m_ytiles / 2) + row;

			TileType cur;
			map_ptr->GetTileType(curtile_wx, curtile_wy, cur);
			curtile->SetTileType(cur);

			ids_per_tiletype[cur].emplace_back(GetTileIdx(col, row));

			for (int aa = 0; aa < close_chars.size(); aa++)
			{
				Character *cur_close_char = close_chars[aa];

				Mesh cur_close_mesh;
				cur_close_mesh.LoadMesh(GetMeshFilename(cur_close_char->GetTileType()));
				cur_close_mesh.UseTexture(GetTextureFilename(cur_close_char->GetTileType()));
				cur_close_mesh.SetWindowSectionPtr(this);
				int col = cur_close_char->GetX();
				int row = cur_close_char->GetY();
				if (curtile_wx == col && curtile_wy == row)
				{
					double ox, oy, w, h;
					curtile->GetRelativeLocation(ox, oy, w, h);
					cur_close_mesh.SetRelativeLocation(ox, oy, w, h);
					glm::mat4 *translations = new glm::mat4[1];
					translations[0] =
					glm::translate(glm::mat4(1.0), 
						glm::vec3(cur_close_mesh.GetXDrawPos_N11(), 
							      cur_close_mesh.GetYDrawPos_N11(), 0));					
					cur_close_mesh.SetUpInstancing(1, m_scale_vec, translations);
					delete[] translations;
					cur_close_mesh.Render();
				}
			}
		}
	}

	int screen_mid_tilex(m_xtiles / 2);
	int screen_mid_tiley(m_ytiles / 2);
	Tile *center_tile = &(m_tiles[GetTileIdx(screen_mid_tilex, screen_mid_tiley)]);
	double ox, oy, w, h;
	center_tile->GetRelativeLocation(ox, oy, w, h);
	Mesh main_char_mesh;
	main_char_mesh.LoadMesh( GetMeshFilename(TileType::MAINCHAR));
	main_char_mesh.UseTexture(GetTextureFilename(TileType::MAINCHAR));
	main_char_mesh.SetWindowSectionPtr(this);
	main_char_mesh.SetRelativeLocation(ox, oy, w, h);
	glm::mat4 *translations = new glm::mat4[1];
	translations[0] =
		glm::translate(glm::mat4(1.0),
			glm::vec3(main_char_mesh.GetXDrawPos_N11(),
				      main_char_mesh.GetYDrawPos_N11(), 0));	
	main_char_mesh.SetUpInstancing(1, m_scale_vec, translations);
	main_char_mesh.Render();
	delete[] translations;
	
	std::map<TileType, std::vector< int > >::iterator it1;
	for (it1 = ids_per_tiletype.begin(); it1 != ids_per_tiletype.end(); it1++)
	{
		Tile *curtiletype = &(m_tiles[it1->second[0]]);
		int n_instances = it1->second.size();
		glm::mat4 *translations = new glm::mat4[n_instances];
		for (int aa = 0; aa < it1->second.size(); aa++)
		{
			Tile *tmp2 = &(m_tiles[it1->second[aa]]);
			translations[aa] =
				glm::translate(glm::mat4(1.0), glm::vec3(tmp2->GetXDrawPos_N11(), tmp2->GetYDrawPos_N11(), 0));
		}
		curtiletype->SetUpInstancing(it1->second.size(), m_scale_vec, translations);
		delete[] translations;
		curtiletype->Render();
	}

	WindowSection::Refresh();
}

int TiledGameBoard::GetTileIdx(int xpos, int ypos)
{
	return (xpos + (ypos * m_xtiles));
}

void TiledGameBoard::Print()
{
	GameMap *map_ptr = GameMap::GetInstance();
	Character *mainchar_ptr = map_ptr->GetMainCharPtr();
	for (int row = m_ytiles - 1; row >= 0; row--)
	{
		for (int col = 0; col < m_xtiles; col++)
		{
			Tile *curtile = &(m_tiles[GetTileIdx(col, row)]);
			int curtile_wx = mainchar_ptr->GetX() - (m_xtiles / 2) + col;
			int curtile_wy = mainchar_ptr->GetY() - (m_ytiles / 2) + row;
			TileType cur;
			map_ptr->GetTileType(curtile_wx, curtile_wy, cur);
			std::cout << static_cast<int>(cur);
			if (col != m_xtiles - 1) std::cout << ",";
		}
		std::cout << std::endl;
	}
}
