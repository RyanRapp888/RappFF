#ifndef GAMEMAP_H__
#define GAMEMAP_H__

#include "Texture.h"
#include <vector>
#include <string>

class Character;

enum class ProxRel
{
   FOUND_TO_LEFT,
   FOUND_TO_RIGHT,
   FOUND_ABOVE,
   FOUND_BELOW
};

class GameMap
{
public:
   GameMap() :m_mainchar_ptr(nullptr), m_xtiles(0), m_ytiles(0) {}
   GameMap(int x_tiles, int y_tiles, Character *mc);
   Character *GetMainCharPtr();
   bool GetTileType(int x, int y, TileType &type);
   void AttachCharacter(Character *otherchar);
   bool HasACharacter(int x, int y);
   bool FindTouchingCharacters(int x, int y, std::vector<Character *> &found_chars, std::vector<ProxRel> &found_prox);
   bool FindCharactersInRange(int minx, int miny, int maxx, int maxy, std::vector<Character *> &found_chars);
   int GetWorldMaxX();
   int GetWorldMaxY();
      
   private:
  
   Character *m_mainchar_ptr;
   std::vector<Character *> m_otherchar_ptrs;
   int m_xtiles;
   int m_ytiles;
   std::vector<TileType> m_tiletypes;
   int GetTileIdx(int xpos, int ypos);
};

#endif
