#include "GameMap.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <direct.h>

using namespace std;

GameMap::GameMap(int x_tiles, int y_tiles)
{
	char cCurrentPath[FILENAME_MAX];

	if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
	{
		return;
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
	printf("The current working directory is %s\n\n", cCurrentPath);


	m_xtiles = x_tiles;
	m_ytiles = y_tiles;
	m_tiletypes.resize(x_tiles * y_tiles);


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

int GameMap::GetWorldMaxX() { return m_xtiles; }

int GameMap::GetWorldMaxY() { return m_ytiles; }

int GameMap::GetTileIdx(int xpos, int ypos)
{
	return (xpos + (ypos * m_xtiles));
}
