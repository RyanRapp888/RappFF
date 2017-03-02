#ifndef CHARACTER_H__
#define CHARACTER_H__

#include "GameMap.h"
#include "CharMotion.h"
#include "Tile.h"
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
	SKELETON,
	BEAR,
	SIZE
};

enum class CharacterState
{
	BEASTMODE,
	GOOD,
	DEAD,
	PETRIFIED,
	SLEEPING,
	CONFUSED,
	BLINDED
};

class CharacterAttributes
{

public:
	CharacterAttributes() :m_hp(50), m_maxhp(150), m_mp(50), m_maxmp(50), m_str(50), m_dodge(50), m_magicskillz(50), m_level(1), m_state(CharacterState::GOOD){}
	int GetHP() const { return m_hp; }
	void SetHP(int dat){ m_hp = dat; }
	int GetMaxHP() const { return m_maxhp; }
	void SetMaxHP(int dat){ m_maxhp = dat; }
	int GetMP() const { return m_mp; }
	void SetMP(int dat){ m_mp = dat; }
	CharacterState GetState() const{ return m_state; }
	bool IsActive() const
	{ 
		bool result(false);
		if (m_state == CharacterState::BEASTMODE || m_state == CharacterState::GOOD || m_state == CharacterState::CONFUSED || m_state == CharacterState::BLINDED)
		{
			result = true;
		}
		
		return result;
	}

private:
	int m_hp;
	int m_maxhp;
	int m_mp;
	int m_maxmp;
	int m_str;
	int m_dodge;
	double m_magicskillz;
	int m_level;
	CharacterState m_state;

};

enum class WeaponType
{
	MELEE,
	RANGED
};

class Weapon
{
public:
	Weapon() {}
	Weapon(const std::string &name, WeaponType wt)
	{
		m_name = name;
		m_type = wt;
		m_critpct = 10;
		m_attack = 10;
		m_accuracy = 20;
	}
	std::string GetName() const { return m_name;}
	void SetName(std::string name){ m_name = name; }
	WeaponType GetType() const { return m_type; }
	void SetType(WeaponType wt){ m_type = wt; }
	int GetCritPct0_100() const { return m_critpct; }
	void SetCritPct0_100(int cp){ m_critpct = cp; }

private:

	std::string m_name;
	WeaponType m_type;
	int m_critpct;
	double m_attack;
	int m_accuracy;
};

enum class ArmorType
{
	HEAD,
	BODY,
	LEGS,
	FEET
};

class Armor
{

public:
	Armor() {}
	Armor(const std::string &name, ArmorType at): m_name(name), m_type(at),
		m_anticrit(10), m_protect(10), m_dodge(20){}
	std::string GetName() const { return m_name; }
	void SetName(std::string name){ m_name = name; }
	ArmorType GetType() const { return m_type; }
	void SetType(ArmorType at){ m_type = at; }
	int GetAntiCritPct0_100() const { return m_anticrit; }
	void SetAntiCritPct0_100(int cp){ m_anticrit = cp; }

private:
	
	std::string m_name;
	ArmorType m_type;
	int m_anticrit;
	int m_protect;
	int m_dodge;
};

enum class UseType
{
	VS_SINGLE,
	VS_AOE,
	FRIENDLY_SINGLE,
	FRIENDLY_AOE,
};

class Item
{
public:
	Item() {}
	Item(const std::string &name, UseType ut) :m_name(name), m_type(ut),
		m_level(1){}
	std::string GetName() const { return m_name; }
	void SetName(std::string name){ m_name = name; }
	UseType GetType() const { return m_type; }
	void SetType(UseType item_t){ m_type = item_t; }
private:
	std::string m_name;
	UseType m_type;
	int m_level;
};

class Spell
{
public:
	Spell() {}
	Spell(const std::string &name, UseType ut) :m_name(name), m_type(ut),
		m_level(1){}
	std::string GetName() const { return m_name; }
	void SetName(std::string name){ m_name = name; }
	UseType GetType() const { return m_type; }
	void SetType(UseType item_t){ m_type = item_t; }
private:
	std::string m_name;
	UseType m_type;
	int m_level;
};

class CharacterInventory
{
public:
	CharacterInventory():m_money(0)
	{
		m_weapons.push_back(Weapon("Sword", WeaponType::MELEE));
		m_armors.push_back(Armor("Helmet #17", ArmorType::HEAD));
		m_items.push_back(Item("Heal Potion", UseType::FRIENDLY_SINGLE));
		m_items.push_back(Item("Mega Heal Potion", UseType::FRIENDLY_AOE));
		m_items.push_back(Item("Throwy Glass", UseType::VS_SINGLE));
		m_items.push_back(Item("Scary Box", UseType::VS_AOE));
		m_spells.push_back(Spell("Hello Spell", UseType::FRIENDLY_SINGLE));
	}
	int GetNWeapons() const { return m_weapons.size(); }
	int GetNArmor() const { return m_armors.size(); }
	int GetNItems() const { return m_items.size(); }
	const Item &GetItemRef(int idx) const { return m_items[idx]; }
	int GetNSpells() const { return m_items.size(); }
private:
	std::vector< Weapon > m_weapons;
	std::vector< Armor > m_armors;
	std::vector< Spell > m_spells;
	std::vector< Item > m_items;
	int m_money;
};

class Character
{
   friend class GameMap;
   
public:

   Character(){}
   std::string GetName() const { return m_name; }
   void SetName(const std::string &name);
   void SetCharacterType(const CharacterType &ctype);
  
   std::string GetHPString() const;
   std::vector<std::string> Character::GetDialogueLines() const;
   
   void SetHP(int dat){ m_attr.SetHP(dat); }
   void SetMaxHP(int dat){ m_attr.SetMaxHP(dat); }
   bool IsActive()const{ return m_attr.IsActive(); }

   int GetNItems() const { return m_inv.GetNItems(); }
   const Item &GetItemRef(int idx) const{ return m_inv.GetItemRef(idx);}
   CharacterType GetCharacterType() const;
   TileType GetTileType() const;
   void SetCharMotion(const CharMotion &cmot);
   bool CanOccupyLocation(int x, int y) const;
   bool SetLocation(int x, int y);
   int GetX() const;
   int GetY() const;

protected:
	
   private:
   
   CharacterAttributes m_attr;
   CharacterInventory m_inv;
   CharacterType m_chartype;
   CharMotion m_char_motion;
   int m_worldx, m_worldy;
   std::string m_name;
};

#endif