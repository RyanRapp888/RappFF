#ifndef GAMEMAP_H__
#define GAMEMAP_H__

#include "Texture.h"
#include <vector>

class GameMap
{
   public:

   GameMap(int x_tiles, int y_tiles);
   bool GetTileType(int x, int y, TileType &type);
   int GetWorldMaxX();
   int GetWorldMaxY();
   
   private:
   
   int m_xtiles;
   int m_ytiles;
   std::vector<TileType> m_tiletypes;
   int GetTileIdx(int xpos, int ypos);
};

#endif
