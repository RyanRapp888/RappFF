#ifndef CHARACTER_H__
#define CHARACTER_H__

#include "GameMap.h"
#include "CharMotion.h"
#include <string>
#include "Tile.h"

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

class Character
{
   friend class GameMap;
   public:

   Character(){}
   void SetName(const std::string &name);
   void SetCharacterType(const CharacterType &ctype);
   CharacterType GetCharacterType();
   TileType GetTileType();
   void SetCharMotion(const CharMotion &cmot);
   bool CanOccupyLocation(int x, int y);
   bool SetLocation(int x, int y);
   int GetX();
   int GetY();
   std::string GetName() { return m_name; }
        
protected:
	
   private:
   
   CharacterType m_chartype;
   std::string m_name;
   CharMotion m_char_motion;
   int m_worldx, m_worldy;
};

#endif