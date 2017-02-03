#ifndef CHARACTER_H__
#define CHARACTER_H__

#include "GameMap.h"
#include "CharMotion.h"
#include "DrawnCharacter.h"
#include <string>
#include "TileType.h"

class Character: public DrawnCharacter
{
   public:

   Character(){}
   Character(const std::string &name, TileType ctype, CharMotion cmot,
             int worldx, int worldy, GameMap *gamemap_ptr): 
			    m_name(name), m_char_motion(cmot),
				m_worldx(worldx), m_worldy(worldy), m_gamemap_ptr(gamemap_ptr)
              { SetTileType(ctype);}
   void SetLocation(int x, int y);
   int GetWorldX();
   int GetWorldY();
   
   private:
   
   std::string m_name;
   CharMotion m_char_motion;
   GameMap *m_gamemap_ptr;
   int m_worldx, m_worldy;
};

#endif