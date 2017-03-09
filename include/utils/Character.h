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
	CharacterAttributes() :m_hp(50), m_maxhp(150), m_mp(50), m_maxmp(50), m_accuracy(50), m_dodge(50), m_level(1), m_state(CharacterState::GOOD), m_flee(50){}
	int GetHP() const { return m_hp; }
	void SetHP(int dat){ m_hp = dat; ValidateHP(); }
	void ModifyHP(int delta)
	{
		m_hp += delta;
    	ValidateHP();
		
	}
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
	bool IsDead() const { return m_state == CharacterState::DEAD; }
	int GetFlee() const{ return m_flee; }
	int GetAccuracy() const{ return m_accuracy; }
	int GetDodge() const{ return m_dodge; }
	void SetFlee(int dat){ m_flee = dat; }
	void SetAccuracy(int dat){ m_accuracy = dat; }
	void SetDodge(int dat){ m_dodge = dat; }

private:
	void ValidateHP()
	{
		if (m_hp > GetMaxHP())
		{
			m_hp = GetMaxHP();
		}
		else if (m_hp <= 0)
		{
			m_state = CharacterState::DEAD;
		}
	}
	int m_hp;
	int m_maxhp;
	int m_mp;
	int m_maxmp;
	int m_accuracy;
	int m_dodge;
	int m_level;
	int m_flee;
	CharacterState m_state;
};

class Weapon
{
public:
	Weapon(): m_name(""), m_accuracy(50) {}
	Weapon(const std::string &name)
	{
		m_name = name;
		m_accuracy = 50;
	}
	std::string GetName() const { return m_name; }
	void SetName(std::string name){ m_name = name; }
	int GetAccuracy() const { return m_accuracy; }
	void SetAccuracy(int dat){ m_accuracy = dat; }

private:

	std::string m_name;
	int m_accuracy;
};

enum class ArmorType
{
	HEAD = 0,
	BODY = 1,
	LEGS = 2,
	FEET = 3
};

class Armor
{
public:
	Armor() :m_name(""),m_type(ArmorType::BODY),m_dodge(50){}
	Armor(const std::string &name, ArmorType at): m_name(name), m_type(at),
		m_dodge(50){}
	std::string GetName() const { return m_name; }
	void SetName(std::string name){ m_name = name; }
	ArmorType GetType() const { return m_type; }
	void SetType(ArmorType at){ m_type = at; }
	int GetDodge() const{ return m_dodge; }
	void SetDodge(int dat){ m_dodge = dat; }
	
private:
	std::string m_name;
	ArmorType m_type;
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
	Item() :m_name(""), m_type(UseType::VS_SINGLE){}
	Item(const std::string &name, UseType ut) :m_name(name), m_type(ut){}
	std::string GetName() const { return m_name; }
	void SetName(std::string name){ m_name = name; }
	UseType GetType() const { return m_type; }
	void SetType(UseType item_t){ m_type = item_t; }
private:
	std::string m_name;
	UseType m_type;
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
		m_weapons.push_back(Weapon("Sword"));
		m_equipped_w_idx = 0;
		m_head_armors.push_back(Armor("Helmet #17", ArmorType::HEAD));
		m_body_armors.push_back(Armor("Body", ArmorType::BODY));
		m_legs_armors.push_back(Armor("Jeans", ArmorType::LEGS));
		m_feet_armors.push_back(Armor("Crocs", ArmorType::FEET));
		m_items.push_back(Item("Heal Potion", UseType::FRIENDLY_SINGLE));
		m_items.push_back(Item("Mega Heal Potion", UseType::FRIENDLY_AOE));
		m_items.push_back(Item("Throwy Glass", UseType::VS_SINGLE));
		m_items.push_back(Item("Scary Box", UseType::VS_AOE));
		m_spells.push_back(Spell("Hello Spell", UseType::FRIENDLY_SINGLE));
	}

	int GetNWeapons() const { return m_weapons.size(); }
	//int GetNArmor() const { return m_armors.size(); }
	int GetNItems() const { return m_items.size(); }
	const Item &GetItemRef(int idx) const { return m_items[idx]; }
	int GetNSpells() const { return m_items.size(); }
	int GetWeaponAccuracy() const
	{
		if (m_equipped_w_idx >= 0)
		{
			return m_weapons[m_equipped_w_idx].GetAccuracy();
		}
		return 1;
	}

	int GetArmorDodge(ArmorType at) const
	{
		int results(0);

		if (at == ArmorType::BODY)
		{
			if (m_equipped_body_idx >= 0)
			{
				results = m_body_armors[m_equipped_body_idx].GetDodge();
			}
			results = 0;
		}
		else if (at == ArmorType::FEET)
		{
			if (m_equipped_feet_idx >= 0)
			{
				results = m_feet_armors[m_equipped_feet_idx].GetDodge();
			}
			results = 0;
		}
		else if (at == ArmorType::HEAD)
		{
			if (m_equipped_head_idx >= 0)
			{
				results = m_head_armors[m_equipped_head_idx].GetDodge();
			}
			results = 0;
		}
		else  //ArmorType::LEGS
		{
			if (m_equipped_legs_idx >= 0)
			{
				results = m_legs_armors[m_equipped_legs_idx].GetDodge();
			}
			results = 0;
		}

		return results;
	}

private:
	int m_equipped_w_idx;
	int m_equipped_head_idx;
	int m_equipped_body_idx;
	int m_equipped_legs_idx;
	int m_equipped_feet_idx;
	std::vector< Weapon > m_weapons;
	std::vector< Armor > m_head_armors;
	std::vector< Armor > m_body_armors;
	std::vector< Armor > m_legs_armors;
	std::vector< Armor > m_feet_armors;
	std::vector< Spell > m_spells;
	std::vector< Item > m_items;
	int m_money;
};

class Character
{

public:

   Character(){}
   friend class GameMap;
   std::string GetName() const { return m_name; }
   std::string GetHPString() const;
   std::vector<std::string> Character::GetDialogueLines() const;
   int GetNItems() const { return m_inv.GetNItems(); }
   const Item &GetItemRef(int idx) const{ return m_inv.GetItemRef(idx); }
   CharacterType GetCharacterType() const;
   TileType GetTileType() const;
   bool IsActive() const { return m_attr.IsActive(); }
   bool IsDead() const { return m_attr.IsDead(); }
   int GetX() const;
   int GetY() const;
   bool CanOccupyLocation(int x, int y) const;
   int GetFlee() const { return m_attr.GetFlee(); }
   int GetAccuracy() const { return m_attr.GetAccuracy(); }
   int GetWeaponAccuracy() const { return m_inv.GetWeaponAccuracy(); }
   int GetDodge() const { return m_attr.GetDodge(); }
   int GetArmorDodge(ArmorType &typ) const{ return m_inv.GetArmorDodge(typ); }

   void ModifyHP(int delta){ m_attr.ModifyHP(delta); }
   void SetName(const std::string &name);
   void SetCharacterType(const CharacterType &ctype);
   void SetHP(int dat){ m_attr.SetHP(dat); }
   void SetMaxHP(int dat){ m_attr.SetMaxHP(dat); }
   void SetCharMotion(const CharMotion &cmot);
   bool SetLocation(int x, int y);
   void SetFlee(int dat){ m_attr.SetFlee(dat); }
   void SetAccuracy(int dat){ m_attr.SetAccuracy(dat);}
   void SetDodge(int dat){ m_attr.SetDodge(dat); }
 
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