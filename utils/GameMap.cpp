#include "GameMap.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <direct.h>
#include "Character.h"

using namespace std;

GameMap::GameMap(int x_tiles, int y_tiles, Character *mc)
{
	m_xtiles = x_tiles;
	m_ytiles = y_tiles;
	m_tiletypes.resize(x_tiles * y_tiles);

	m_mainchar_ptr = mc;
	mc->SetGameMapPtr(this);

	std::ifstream input_file("res\\map.csv");
	if (!input_file.is_open())
	{
		std::cout << "ERROR: Unable to load map file (map.csv)" << std::endl;
		return;
	}

	for (int row = y_tiles - 1; row >= 0; row--)
	{
		std::string line;
		std::getline(input_file, line);
		if (!input_file.good())
			break;

		std::stringstream iss(line);

		for (int col = 0; col < x_tiles; col++)
		{
			std::string val;
			std::getline(iss, val, ',');
			if (!iss.good())
				break;

			std::stringstream tmp_sstr(val);
			int tmpint;
			tmp_sstr >> tmpint;
			m_tiletypes[GetTileIdx(col, row)] = static_cast<TileType>(tmpint);
		}
	}
	input_file.close();
}

Character *GameMap::GetMainCharPtr()
{
	return m_mainchar_ptr;
}


void GameMap::AttachCharacter(Character *otherchar)
{
	
	m_otherchar_ptrs.emplace_back(otherchar);
}

bool GameMap::FindCharactersInRange(int minx, int miny, int maxx, int maxy, std::vector<Character *> &found_chars)
{
	found_chars.clear();
	for (int aa = 0; aa < m_otherchar_ptrs.size(); aa++)
	{
		Character *cur = m_otherchar_ptrs[aa];
		if (cur->GetX() >= minx && cur->GetX() <= maxx &&
			cur->GetY() >= miny && cur->GetY() <= maxy)
		{
			found_chars.push_back(cur);
		}
	}
	return (found_chars.size() > 0);
}

bool GameMap::GetTileType(int x, int y, TileType &type)
{
	if (x < 0 || x >= m_xtiles ||
		y < 0 || y >= m_ytiles)
	{
		type = TileType::BLACK;
		return true;
	}
	int idx = GetTileIdx(x, y);
	if (idx < 0 || idx >= m_tiletypes.size())
	{
		type = TileType::BLACK;
		return false;
	}

	type = m_tiletypes[idx];
	return true;
}

bool GameMap::HasACharacter(int x, int y)
{
	for (int aa = 0; aa < m_otherchar_ptrs.size(); aa++)
	{
		Character *cur = m_otherchar_ptrs[aa];
		if (cur->GetX() == x && cur->GetY() == y)
		{
			return true;
		}
	}
	return false;
}

bool GameMap::FindTouchingCharacters(int x, int y, std::vector<Character *> &found_chars, std::vector<ProxRel> &found_prox)
{
	for (int aa = 0; aa < m_otherchar_ptrs.size(); aa++)
	{
		Character *cur = m_otherchar_ptrs[aa];
		if (cur->GetX() == (x-1) && cur->GetY() == y)
		{
			found_chars.push_back(cur);
			found_prox.push_back(ProxRel::FOUND_TO_LEFT);
		}
		else if(cur->GetX() == (x+1) && cur->GetY() == y)
		{
			found_chars.push_back(cur);
			found_prox.push_back(ProxRel::FOUND_TO_RIGHT);
		}
		else if (cur->GetX() == x && cur->GetY() == (y + 1))
		{
			found_chars.push_back(cur);
			found_prox.push_back(ProxRel::FOUND_ABOVE);
		}
		else if (cur->GetX() == x && cur->GetY() == (y - 1))
		{
			found_chars.push_back(cur);
			found_prox.push_back(ProxRel::FOUND_BELOW);
		}
	}
	return (found_chars.size() > 0);
}

int GameMap::GetWorldMaxX() { return m_xtiles; }

int GameMap::GetWorldMaxY() { return m_ytiles; }

int GameMap::GetTileIdx(int xpos, int ypos)
{
	return (xpos + (ypos * m_xtiles));
}
