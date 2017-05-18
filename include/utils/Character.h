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
	CharacterAttributes() :m_hp(16), m_maxhp(16), m_mp(0), m_maxmp(0), m_accuracy(3), m_dodge(3), m_level(1), m_state(CharacterState::GOOD), m_flee(3), 
	                       m_attack_skill(3),m_dmg_absorb(3){}
	int GetHP() const;
	void SetHP(int dat);
	void ModifyHP(int delta);
	int GetMaxHP() const;
	void SetMaxHP(int dat);
	int GetMP() const;
	void SetMP(int dat);
	CharacterState GetState() const;
	bool IsActive() const;
	bool IsDead() const;
	int GetFlee() const; 
	int GetAccuracy() const; 
	int GetDodge() const; 
	int GetAttackSkill() const;
	int GetDamageAbsorb() const; 
	void SetFlee(int dat);
	void SetAccuracy(int dat);
	void SetDodge(int dat);
	void SetAttackSkill(int dat);
	void SetDamageAbsorb(int dat);

private:
	void ValidateHP();
	int m_hp;
	int m_maxhp;
	int m_mp;
	int m_maxmp;
	int m_accuracy;
	int m_dodge;
	int m_level;
	int m_flee;
	int m_attack_skill;
	int m_dmg_absorb; // absorption
	CharacterState m_state;
};

class Weapon
{
public:
	Weapon(): m_name(""), m_accuracy(3), m_dmg(3) {}
	Weapon(const std::string &name);
	std::string GetName() const; 
	int GetWeaponDamage() const;
	void SetName(std::string name);
	int GetAccuracy() const;
	void SetAccuracy(int dat);
	void SetWeaponDamage(int dat);

private:

	std::string m_name;
	int m_accuracy;
	int m_dmg;
};

enum class ArmorType
{
	HEAD = 0,
	BODY = 1,
	LEGS = 2,
	FEET = 3,
	INACTIVE = 4
};

class Armor
{
public:
	Armor() :m_name(""),m_type(ArmorType::BODY),m_dodge(3),m_absorb(3){}
	Armor(const std::string &name, ArmorType at): m_name(name), m_type(at),
		m_dodge(3), m_absorb(3){}
	
	std::string GetName() const;
	ArmorType GetType() const;
	int GetDodge() const;
	int GetAbsorb() const;
	void SetName(std::string name);
	void SetType(ArmorType at);
	void SetDodge(int dat);
	void SetAbsorb(int dat);
	
private:
	std::string m_name;
	ArmorType m_type;
	int m_dodge;
	int m_absorb;
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
	std::string GetName() const;
	void SetName(std::string name);
	UseType GetType() const;
	void SetType(UseType item_t);
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
	std::string GetName() const;
	void SetName(std::string name);
	UseType GetType() const;
	void SetType(UseType item_t);
private:
	std::string m_name;
	UseType m_type;
	int m_level;
};

class CharacterInventory
{
public:
	CharacterInventory();
	size_t GetNWeapons() const;
	size_t GetNItems() const;
	const Item &GetItemRef(int idx) const;
	size_t GetNSpells() const;
	int GetWeaponAccuracy() const;
	int GetWeaponDamage() const;
	int GetArmorDodge(ArmorType at) const;
	int GetArmorAbsorb(ArmorType at) const;

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
 
   // Character Identity Stuff
   std::string GetName() const;
   std::string GetHPString() const;
   std::vector<std::string> Character::GetDialogueLines() const;
   size_t GetNItems() const;
   const Item &GetItemRef(int idx) const;
   CharacterType GetCharacterType() const;
   TileType GetTileType() const;
   int GetX() const;
   int GetY() const;
   void SetName(const std::string &name);
   void SetHP(int dat);
   void SetMaxHP(int dat);
   void SetCharacterType(const CharacterType &ctype);
   void SetCharMotion(const CharMotion &cmot);
   bool SetLocation(int x, int y);

   // Character attribute & inventory stuff
   int GetFlee() const;
   int GetAccuracy() const;
   int GetWeaponAccuracy() const;
   int GetWeaponDamage() const;
   int GetAttackSkill() const;
   int GetDodge() const;
   int GetAbsorb() const;
   int GetArmorDodge(ArmorType &typ) const;
   int GetArmorAbsorb(ArmorType &typ) const;
   
   void SetFlee(int dat);
   void SetAccuracy(int dat);
   void SetAttackSkill(int dat);
   void SetDodge(int dat);
   void SetAbsorb(int dat);
   
   void ModifyHP(int delta);
   bool IsActive() const;
   bool IsDead() const;
   bool CanOccupyLocation(int x, int y) const;

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