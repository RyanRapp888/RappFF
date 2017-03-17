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
	static GameMap *GetInstance();
  
   GameMap() :m_mainchar_ptr(nullptr), m_xtiles(0), m_ytiles(0) {}
   bool LoadGameMap(int x_tiles, int y_tiles);

   bool AttachMainCharacter(Character *mainchar);
   void AttachCharacter(Character *otherchar);
   void AddToHeroParty(Character *hero);
   void RemoveFromHeroParty(Character *hero);

   Character *GetMainCharPtr();
   void GetCurHeroes(std::vector<Character *> &heroes);
   bool GetTileType(int x, int y, TileType &type);

   bool HasACharacter(int x, int y);
   bool FindTouchingCharacter(int x, int y, Character *&found_char, ProxRel &found_prox);
   bool FindCharactersInRange(int minx, int miny, int maxx, int maxy, std::vector<Character *> &found_chars);
   int GetWorldMaxX();
   int GetWorldMaxY();
   int GetMobOdds(int x, int y);
   std::vector<Character> GetMobs(int x, int y);
      
   private:
   
   static bool m_initialized;
   static GameMap *m_instance;
   Character *m_mainchar_ptr;
   std::vector<Character *> m_otherchar_ptrs;
   std::vector<Character *> m_cur_heroes;
   int m_xtiles;
   int m_ytiles;
   std::vector<TileType> m_tiletypes;
   int GetTileIdx(int xpos, int ypos);
};

#endif
