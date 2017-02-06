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
   friend class GameMap;
   public:

   Character(){}
   Character(const std::string &name, CharacterType ctype, CharMotion cmot, int worldx, int worldy);
   bool CanOccupyLocation(int x, int y);
   void SetLocation(int x, int y);
   int GetX();
   int GetY();
   std::string GetName() { return m_name; }
   Character &operator=(const Character & other);
     
protected:
	void SetGameMapPtr(GameMap *gm_ptr);

   private:
   
   CharacterType m_chartype;
   std::string m_name;
   CharMotion m_char_motion;
   GameMap *m_gamemap_ptr;
   int m_worldx, m_worldy;
};

#endif