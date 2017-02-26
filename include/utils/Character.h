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
	SKELETON,
	WEASEL,
	SIZE
};

class CharacterAttributes
{
public:
	CharacterAttributes() :m_hp(50), m_mp(50), m_str(50), m_dodge(50), m_magicskillz(50), m_level(1){}

private:
	int  m_hp;
	int  m_mp;
	int  m_str;
	int  m_dodge;
	double m_magicskillz;
	int m_level;
};

enum class WeaponType
{
	Melee,
	Ranged
};

class Weapon
{
public:
	Weapon() {}
private:
	std::string m_name;
	WeaponType m_type;
	int m_critpct;
	double m_attack;
	int m_accuracy;
};

enum class ArmorType
{
	Head,
	Body,
	Legs
};

class Armor
{
public:
	Armor() {}
private:
	std::string m_name;
	ArmorType m_type;
	int m_anticrit;
	int m_protect;
	int m_dodge;
};

enum class SpellType
{
	VsSingle,
	AOE,
	Friendly,
	Mean
};

class Spell
{
public:
	Spell() {}
private:
	std::string m_name;
	SpellType m_type;
	int m_level;
};

class CharacterInventory
{
public:
	CharacterInventory():m_money(0) {}

private:
	std::vector< Weapon> m_weapons;
	std::vector< Armor> m_armor;
	std::vector< Spell> m_spells;
	int m_money;
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
   
   CharacterAttributes m_attr;
   CharacterInventory m_inv;
   CharacterType m_chartype;
   std::string m_name;
   CharMotion m_char_motion;
   int m_worldx, m_worldy;
};

#endif