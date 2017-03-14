#include "GameMap.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <direct.h>
#include "Character.h"
#include <stdlib.h>

using namespace std;

bool GameMap::m_initialized = false;
GameMap *GameMap::m_instance = 0;

GameMap *GameMap::GetInstance()
{
	if (!m_initialized)
	{
		m_instance = new GameMap();
		m_initialized = true;
	}
	return m_instance;
}

int GameMap::GetMonsterOdds(int x, int y)
{
	TileType cc;
	GetTileType(x, y,cc);
	if (cc == TileType::BRICKS || cc == TileType::PLANK)
	{
		return -1;
	}
	else
	{
		return 50;
	}
	return -1;
}

bool GameMap::AttachMainCharacter(Character *mainchar)
{
	if (mainchar == nullptr) return false;
	m_mainchar_ptr = mainchar;
	return true;
}

void GameMap::GetCurHeroes(std::vector<Character *> &heroes)
{
	heroes.clear();

	for (auto curhero : m_cur_heroes)
	{
		heroes.emplace_back(curhero);
	}
}

void GameMap::AddToHeroParty(Character *hero)
{
	for (int aa = 0; aa < m_cur_heroes.size(); aa++)
	{
		Character *cur = m_cur_heroes[aa];
		if (hero == cur)
		{
			return;
		}
	}
	m_cur_heroes.push_back(hero);
}

void GameMap::RemoveFromHeroParty(Character *hero)
{
	for (int aa = 0; aa < m_cur_heroes.size(); aa++)
	{
		Character *cur = m_cur_heroes[aa];
		if (hero == cur)
		{
			m_cur_heroes.erase(m_cur_heroes.begin() + aa);
		}
	}
}

bool GameMap::LoadGameMap(int x_tiles, int y_tiles)
{
	m_xtiles = x_tiles;
	m_ytiles = y_tiles;
	m_tiletypes.resize(x_tiles * y_tiles);

	std::ifstream input_file("res\\map.csv");
	if (!input_file.is_open())
	{
		std::cout << "ERROR: Unable to load map file (map.csv)" << std::endl;
		return false;
	}

	for (int row = y_tiles - 1; row >= 0; row--)
	{
		std::string line;
		std::getline(input_file, line);
		
		std::stringstream iss(line);

		for (int col = 0; col < x_tiles; col++)
		{
			std::string val;
			std::getline(iss, val, ',');
			
			std::stringstream tmp_sstr(val);
			int tmpint;
			tmp_sstr >> tmpint;
			m_tiletypes[GetTileIdx(col, row)] = static_cast<TileType>(tmpint);
		}
	}
	input_file.close();
	return true;
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
		type = TileType::WATER;
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

bool GameMap::FindTouchingCharacter(int x, int y, Character *&found_char, ProxRel &found_prox)
{
	for (int aa = 0; aa < m_otherchar_ptrs.size(); aa++)
	{
		Character *cur = m_otherchar_ptrs[aa];
		if (cur->GetX() == (x-1) && cur->GetY() == y)
		{
			found_char = cur;
			found_prox = ProxRel::FOUND_TO_LEFT;
			return true;
		}
		else if(cur->GetX() == (x+1) && cur->GetY() == y)
		{
			found_char = cur;
			found_prox = ProxRel::FOUND_TO_RIGHT;
			return true;
		}
		else if (cur->GetX() == x && cur->GetY() == (y + 1))
		{
			found_char = cur;
			found_prox = ProxRel::FOUND_ABOVE;
			return true;
		}
		else if (cur->GetX() == x && cur->GetY() == (y - 1))
		{
			found_char = cur;
			found_prox = ProxRel::FOUND_BELOW;
			return true;
		}
	}
	return false;
}

int GameMap::GetWorldMaxX() { return m_xtiles; }

int GameMap::GetWorldMaxY() { return m_ytiles; }

int GameMap::GetTileIdx(int xpos, int ypos)
{
	return (xpos + (ypos * m_xtiles));
}

std::vector<Character> GameMap::GetMonsters(int x, int y)
{
	std::vector<Character> results;
	results.resize(4);
	results[0].SetName("Skeleton");
	results[0].SetCharacterType(CharacterType::SKELETON);
	//results[0].
	results[0].SetHP(20); 
	results[0].SetMaxHP(20);
	results[0].SetLocation(-99, -99);

	results[1].SetName("Skeleton");
	results[1].SetCharacterType(CharacterType::SKELETON);
	results[1].SetHP(20);
	results[1].SetMaxHP(20);
	results[1].SetLocation(-99, -99);

	results[2].SetName("Enraged Bear");
	results[2].SetCharacterType(CharacterType::BEAR);
	results[2].SetHP(20);
	results[2].SetMaxHP(20);
	results[2].SetLocation(-99, -99);

	results[3].SetName("Magic Bear");
	results[3].SetCharacterType(CharacterType::BEAR);
	results[3].SetHP(20);
	results[3].SetMaxHP(20);
	results[3].SetLocation(-99, -99);
	return results;
}