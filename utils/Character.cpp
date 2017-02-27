#include "Character.h"
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <sstream>

bool permission_initialized = 0;
static std::map< CharacterType, std::vector<TileType> > travel_permissions;

static std::map<CharacterType, TileType> CTypeToTileType
{
	{ CharacterType::EMMY, TileType::EMMY },
	{ CharacterType::FAIRY, TileType::FAIRY },
	{ CharacterType::JELLYBEAN, TileType::JELLYBEAN },
	{ CharacterType::MAINCHAR, TileType::MAINCHAR },
	{ CharacterType::MERMAID, TileType::MERMAID },
	{ CharacterType::OCTOPUS, TileType::OCTOPUS },
	{ CharacterType::PRINCESS, TileType::PRINCESS }
};

void Character::SetName(const std::string &name)
{
	m_name = name;
}

void Character::SetCharacterType(const CharacterType &ctype)
{
	m_chartype = ctype;
}

CharacterType Character::GetCharacterType()
{
	return m_chartype;
}

TileType Character::GetTileType()
{
	return CTypeToTileType[m_chartype];
}

void Character::SetCharMotion(const CharMotion &cmot)
{
	m_char_motion = cmot;
}
	
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

bool Character::CanOccupyLocation(int x, int y)
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

bool Character::SetLocation(int x, int y)
{
	bool result;
	GameMap *map_ptr = GameMap::GetInstance();
	if (map_ptr == nullptr) return false;
	if (x < 0)
	{
		x = 0;
		return false;
	}
	
	if (x >= map_ptr->GetWorldMaxX())
	{
		x = map_ptr->GetWorldMaxX() - 1;
		return false;
	}

	if (y < 0)
	{
		y = 0;
		return false;
	}

	if (y >= map_ptr->GetWorldMaxY())
	{
		y = map_ptr->GetWorldMaxY() - 1;
		return false;
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

int Character::GetX() { return m_worldx; }

int Character::GetY() { return m_worldy; }

std::string Character::GetHPString()
{
	std::ostringstream aa;
	aa << m_attr.GetHP() << " / " << m_attr.GetMaxHP();
	return aa.str();
}

std::vector<std::string> Character::GetDialogueLines()
{
	std::vector<std::string> results;
	results.emplace_back("Hi! I'm " + GetName());
	results.emplace_back("My HP is " + GetHPString());
	results.emplace_back("You should check out that weird DIRT DIAMOND.");
	return results;
}