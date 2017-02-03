#ifndef CHARACTER_H__
#define CHARACTER_H__

#include "GameMap.h"
#include "CharMotion.h"
#include "DrawnCharacter.h"
#include <string>

enum class CharacterType
{
	EMMY,
	FAIRY,
	JELLYBEAN,
	MAINCHAR,
	MERMAID,
	OCTOPUS,
	PRINCESS,
	SIZE
};

class Character: public DrawnCharacter
{
   public:

   Character(){}
   Character(const std::string &name, CharacterType ctype, CharMotion cmot,
	   int worldx, int worldy, GameMap *gamemap_ptr);
   bool CanOccupyTileType(TileType tt);
   void SetLocation(int x, int y);
   int GetWorldX();
   int GetWorldY();
   
   private:
   
   CharacterType m_chartype;
   std::string m_name;
   CharMotion m_char_motion;
   GameMap *m_gamemap_ptr;
   int m_worldx, m_worldy;
};

#endif