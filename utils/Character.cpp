#include "Character.h"
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <sstream>


bool permission_initialized = 0;
static std::map< CharacterType, std::vector<TileType> > travel_permissions;


static void InitTravelPermissions()
{
	std::vector<TileType> typicalallowed = std::vector< TileType >{
		TileType::BRICKS, TileType::DESERT, TileType::GRASS, TileType::GRASS2,
			TileType::MUD, TileType::PLANK, TileType::ROCKS, TileType::WOOD };

	std::copy(typicalallowed.begin(), typicalallowed.end(), std::back_inserter(travel_permissions[CharacterType::EMMY]));
	travel_permissions[CharacterType::EMMY].emplace_back(TileType::MTN);
	travel_permissions[CharacterType::EMMY].emplace_back(TileType::MTNSNOW);

	std::copy(typicalallowed.begin(), typicalallowed.end(), std::back_inserter(travel_permissions[CharacterType::FAIRY]));
	travel_permissions[CharacterType::FAIRY].emplace_back(TileType::MTN);
	travel_permissions[CharacterType::FAIRY].emplace_back(TileType::MTNSNOW);

	std::copy(typicalallowed.begin(), typicalallowed.end(), std::back_inserter(travel_permissions[CharacterType::JELLYBEAN]));
	std::copy(typicalallowed.begin(), typicalallowed.end(), std::back_inserter(travel_permissions[CharacterType::MAINCHAR]));

	travel_permissions[CharacterType::MERMAID].emplace_back(TileType::WATER);
	travel_permissions[CharacterType::OCTOPUS].emplace_back(TileType::WATER);

	std::copy(typicalallowed.begin(), typicalallowed.end(), std::back_inserter(travel_permissions[CharacterType::PRINCESS]));
	permission_initialized = true;
}

static std::map<CharacterType, TileType> CTypeToTileType
{
	{ CharacterType::EMMY, TileType::EMMY },
	{ CharacterType::FAIRY, TileType::FAIRY },
	{ CharacterType::JELLYBEAN, TileType::JELLYBEAN },
	{ CharacterType::MAINCHAR, TileType::MAINCHAR },
	{ CharacterType::MERMAID, TileType::MERMAID },
	{ CharacterType::OCTOPUS, TileType::OCTOPUS },
	{ CharacterType::PRINCESS, TileType::PRINCESS },
	{ CharacterType::SKELETON, TileType::SKELETON },
		{ CharacterType::BEAR, TileType::BEAR }
};

int CharacterAttributes::GetHP() const { return m_hp; }
void CharacterAttributes::SetHP(int dat){ m_hp = dat; ValidateHP(); }
void CharacterAttributes::ModifyHP(int delta)
{
	m_hp += delta;
	ValidateHP();
}
int CharacterAttributes::GetMaxHP() const { return m_maxhp; }
void CharacterAttributes::SetMaxHP(int dat){ m_maxhp = dat; }
int CharacterAttributes::GetMP() const { return m_mp; }
void CharacterAttributes::SetMP(int dat){ m_mp = dat; }
CharacterState CharacterAttributes::GetState() const{ return m_state; }
bool CharacterAttributes::IsDead() const { return m_state == CharacterState::DEAD; }
int CharacterAttributes::GetFlee() const{ return m_flee; }
int CharacterAttributes::GetAccuracy() const{ return m_accuracy; }
int CharacterAttributes::GetDodge() const{ return m_dodge; }
int CharacterAttributes::GetAttackSkill() const{ return m_attack_skill; }
int CharacterAttributes::GetDamageAbsorb() const{ return m_dmg_absorb; }
void CharacterAttributes::SetFlee(int dat){ m_flee = dat; }
void CharacterAttributes::SetAccuracy(int dat){ m_accuracy = dat; }
void CharacterAttributes::SetDodge(int dat){ m_dodge = dat; }
void CharacterAttributes::SetAttackSkill(int dat){ m_attack_skill = dat; }
void CharacterAttributes::SetDamageAbsorb(int dat){ m_dmg_absorb = dat; }
void CharacterAttributes::ValidateHP()
{
	if (m_hp > GetMaxHP())
	{
		m_hp = GetMaxHP();
	}
	else if (m_hp <= 0)
	{
		m_state = CharacterState::DEAD;
		m_hp = 0;
	}
}

bool CharacterAttributes::IsActive() const
{
	bool result(false);
	if (m_state == CharacterState::BEASTMODE || m_state == CharacterState::GOOD || m_state == CharacterState::CONFUSED || m_state == CharacterState::BLINDED)
	{
		result = true;
	}

	return result;
}

Weapon::Weapon(const std::string &name)
{
	m_name = name;
	m_accuracy = 3;
	m_dmg = 3;
}

std::string Weapon::GetName() const { return m_name; }
int Weapon::GetWeaponDamage() const { return m_dmg; }
void Weapon::SetName(std::string name){ m_name = name; }
int Weapon::GetAccuracy() const { return m_accuracy; }
void Weapon::SetAccuracy(int dat){ m_accuracy = dat; }
void Weapon::SetWeaponDamage(int dat){ m_dmg = dat; }


std::string Armor::GetName() const { return m_name; }
ArmorType Armor::GetType() const { return m_type; }
int Armor::GetDodge() const{ return m_dodge; }
int Armor::GetAbsorb() const{ return m_absorb; }
void Armor::SetName(std::string name){ m_name = name; }
void Armor::SetType(ArmorType at){ m_type = at; }
void Armor::SetDodge(int dat){ m_dodge = dat; }
void Armor::SetAbsorb(int dat){ m_absorb = dat; }


std::string Item::GetName() const { return m_name; }
void Item::SetName(std::string name){ m_name = name; }
UseType Item::GetType() const { return m_type; }
void Item::SetType(UseType item_t){ m_type = item_t; }

std::string Spell::GetName() const { return m_name; }
void Spell::SetName(std::string name){ m_name = name; }
UseType Spell::GetType() const { return m_type; }
void Spell::SetType(UseType item_t){ m_type = item_t; }

CharacterInventory::CharacterInventory():m_money(0)
{
	m_weapons.push_back(Weapon("Sword"));
	m_equipped_w_idx = 0;
	m_head_armors.push_back(Armor("Helmet #17", ArmorType::HEAD));
	m_body_armors.push_back(Armor("Body", ArmorType::BODY));
	m_legs_armors.push_back(Armor("Jeans", ArmorType::LEGS));
	m_feet_armors.push_back(Armor("Crocs", ArmorType::FEET));
	m_equipped_body_idx = 0;
	m_equipped_feet_idx = 0;
	m_equipped_head_idx = 0;
	m_equipped_legs_idx = 0;
	m_items.push_back(Item("Heal Potion", UseType::FRIENDLY_SINGLE));
	m_items.push_back(Item("Mega Heal Potion", UseType::FRIENDLY_AOE));
	m_items.push_back(Item("Throwy Glass", UseType::VS_SINGLE));
	m_items.push_back(Item("Scary Box", UseType::VS_AOE));
	m_spells.push_back(Spell("Hello Spell", UseType::FRIENDLY_SINGLE));
}
int CharacterInventory::GetNWeapons() const { return m_weapons.size(); }
int CharacterInventory::GetNItems() const { return m_items.size(); }
const Item &CharacterInventory::GetItemRef(int idx) const { return m_items[idx]; }
int CharacterInventory::GetNSpells() const { return m_items.size(); }
int CharacterInventory::GetWeaponAccuracy() const
{
	if (m_equipped_w_idx >= 0)
	{
		return m_weapons[m_equipped_w_idx].GetAccuracy();
	}
	return 1;
}

int CharacterInventory::GetWeaponDamage() const
{
	if (m_equipped_w_idx >= 0)
	{
		return m_weapons[m_equipped_w_idx].GetWeaponDamage();
	}
	return 1;
}

int CharacterInventory::GetArmorDodge(ArmorType at) const
{
	int results(0);

	if (at == ArmorType::BODY)
	{
		if (m_equipped_body_idx >= 0)
		{
			results = m_body_armors[m_equipped_body_idx].GetDodge();
		}
	}
	else if (at == ArmorType::FEET)
	{
		if (m_equipped_feet_idx >= 0)
		{
			results = m_feet_armors[m_equipped_feet_idx].GetDodge();
		}
	}
	else if (at == ArmorType::HEAD)
	{
		if (m_equipped_head_idx >= 0)
		{
			results = m_head_armors[m_equipped_head_idx].GetDodge();
		}
	}
	else if (at == ArmorType::LEGS)
	{
		if (m_equipped_legs_idx >= 0)
		{
			results = m_legs_armors[m_equipped_legs_idx].GetDodge();
		}
	}
	else if (at == ArmorType::INACTIVE)
	{
		// If armor is inactive, we take the worst piece of equipped armor and divide it by 2
		int min(100);
		int body = GetArmorDodge(ArmorType::BODY);
		min = body < min ? body : min;
		int feet = GetArmorDodge(ArmorType::FEET);
		min = feet < min ? feet : min;
		int head = GetArmorDodge(ArmorType::HEAD);
		min = head < min ? head : min;
		int legs = GetArmorDodge(ArmorType::LEGS);
		min = legs < min ? legs : min;
		return min / 2;
	}

	return results;
}

int CharacterInventory::GetArmorAbsorb(ArmorType at) const
{
	int results(0);

	if (at == ArmorType::BODY)
	{
		if (m_equipped_body_idx >= 0)
		{
			results = m_body_armors[m_equipped_body_idx].GetAbsorb();
		}
	}
	else if (at == ArmorType::FEET)
	{
		if (m_equipped_feet_idx >= 0)
		{
			results = m_feet_armors[m_equipped_feet_idx].GetAbsorb();
		}
	}
	else if (at == ArmorType::HEAD)
	{
		if (m_equipped_head_idx >= 0)
		{
			results = m_head_armors[m_equipped_head_idx].GetAbsorb();
		}
	}
	else if (at == ArmorType::LEGS)
	{
		if (m_equipped_legs_idx >= 0)
		{
			results = m_legs_armors[m_equipped_legs_idx].GetAbsorb();
		}
	}
	else if (at == ArmorType::INACTIVE)
	{
		// If armor is inactive, we take the worst piece of equipped armor and divide it by 2
		int min(100);
		int body = GetArmorAbsorb(ArmorType::BODY);
		min = body < min ? body : min;
		int feet = GetArmorAbsorb(ArmorType::FEET);
		min = feet < min ? feet : min;
		int head = GetArmorAbsorb(ArmorType::HEAD);
		min = head < min ? head : min;
		int legs = GetArmorAbsorb(ArmorType::LEGS);
		min = legs < min ? legs : min;
		return min / 2;
	}
	return results;
}

// Character Identity Stuff
std::string Character::GetName() const { return m_name; }

std::string Character::GetHPString() const
{
	std::ostringstream aa;
	aa << m_attr.GetHP() << " / " << m_attr.GetMaxHP();
	return aa.str();
}

std::vector<std::string> Character::GetDialogueLines() const
{
	std::vector<std::string> results;
	results.emplace_back("Hi! I'm " + GetName());
	results.emplace_back("My HP is " + GetHPString());
	results.emplace_back("You should check out that weird DIRT DIAMOND.");
	return results;
}

int Character::GetNItems() const { return m_inv.GetNItems(); }
const Item &Character::GetItemRef(int idx) const{ return m_inv.GetItemRef(idx); }
CharacterType Character::GetCharacterType() const
{
	return m_chartype;
}

TileType Character::GetTileType() const
{
	return CTypeToTileType[m_chartype];
}

int Character::GetX() const { return m_worldx; }

int Character::GetY() const { return m_worldy; }
void Character::SetName(const std::string &name)
{
	m_name = name;
}
void Character::SetHP(int dat){ m_attr.SetHP(dat); }
void Character::SetMaxHP(int dat){ m_attr.SetMaxHP(dat); }
void Character::SetCharacterType(const CharacterType &ctype)
{
	m_chartype = ctype;
}
void Character::SetCharMotion(const CharMotion &cmot)
{
	m_char_motion = cmot;
}

bool Character::SetLocation(int x, int y)
{
	GameMap *map_ptr = GameMap::GetInstance();
	if (map_ptr == nullptr) return false;
	
	if (x < 0)
	{
		x = 0;
	}
	else if (x >= map_ptr->GetWorldMaxX())
	{
		x = map_ptr->GetWorldMaxX() - 1;
	}

	if (y < 0)
	{
		y = 0;
	}
	else if (y >= map_ptr->GetWorldMaxY())
	{
		y = map_ptr->GetWorldMaxY() - 1;
	}

	//CanOccupyLocation looks to see if our character can step on the desired
	//tile type AND makes sure its not occupied.
	if (CanOccupyLocation(x, y))
	{
		m_worldx = x;
		m_worldy = y;
		return true;
	}
	return false;
}

// Character attribute & inventory stuff
int Character::GetFlee() const { return m_attr.GetFlee(); }
int Character::GetAccuracy() const { return m_attr.GetAccuracy(); }
int Character::GetWeaponAccuracy() const { return m_inv.GetWeaponAccuracy(); }
int Character::GetWeaponDamage() const { return m_inv.GetWeaponDamage(); }
int Character::GetAttackSkill() const { return m_attr.GetAttackSkill(); }
int Character::GetDodge() const { return m_attr.GetDodge(); }
int Character::GetAbsorb() const { return m_attr.GetDamageAbsorb(); }
int Character::GetArmorDodge(ArmorType &typ) const{ return m_inv.GetArmorDodge(typ); }
int Character::GetArmorAbsorb(ArmorType &typ) const{ return m_inv.GetArmorAbsorb(typ); }

void Character::SetFlee(int dat){ m_attr.SetFlee(dat); }
void Character::SetAccuracy(int dat){ m_attr.SetAccuracy(dat); }
void Character::SetAttackSkill(int dat){ m_attr.SetAttackSkill(dat); }
void Character::SetDodge(int dat){ m_attr.SetDodge(dat); }
void Character::SetAbsorb(int dat){ m_attr.SetDamageAbsorb(dat); }

void Character::ModifyHP(int delta){ m_attr.ModifyHP(delta); }
bool Character::IsActive() const { return m_attr.IsActive(); }
bool Character::IsDead() const { return m_attr.IsDead(); }
bool Character::CanOccupyLocation(int x, int y) const
{
	if (!permission_initialized) InitTravelPermissions();

	TileType typ;
	GameMap *map_ptr = GameMap::GetInstance();
	map_ptr->GetTileType(x, y, typ);
	//First, we see if our character is allowed to be on the given tiletype
	std::vector<TileType> &vv = travel_permissions[m_chartype];
	auto iter = std::find(vv.begin(), vv.end(), typ);
	if (iter == vv.end())
	{
		return false;
	}

	//then we make sure no other players are occupying the space
	return !(map_ptr->HasACharacter(x, y));
}





/*

// Character attribute & inventory stuff
int Character::GetFlee() const { return m_attr.GetFlee(); }
int Character::GetAccuracy() const { return m_attr.GetAccuracy(); }
int Character::GetWeaponAccuracy() const { return m_inv.GetWeaponAccuracy(); }
int Character::GetWeaponDamage() const { return m_inv.GetWeaponDamage(); }
int Character::GetAttackSkill() const { return m_attr.GetAttackSkill(); }
int Character::GetDodge() const { return m_attr.GetDodge(); }
int Character::GetAbsorb() const { return m_attr.GetDamageAbsorb(); }
int Character::GetArmorDodge(ArmorType &typ) const{ return m_inv.GetArmorDodge(typ); }
int Character::GetArmorAbsorb(ArmorType &typ) const{ return m_inv.GetArmorAbsorb(typ); }

void Character::SetFlee(int dat){ m_attr.SetFlee(dat); }
void Character::SetAccuracy(int dat){ m_attr.SetAccuracy(dat); }
void Character::SetAttackSkill(int dat){ m_attr.SetAttackSkill(dat); }
void Character::SetDodge(int dat){ m_attr.SetDodge(dat); }
void Character::SetAbsorb(int dat){ m_attr.SetDamageAbsorb(dat); }

void Character::ModifyHP(int delta){ m_attr.ModifyHP(delta); }
bool Character::IsActive() const { return m_attr.IsActive(); }
bool Character::IsDead() const { return m_attr.IsDead(); }


*/
