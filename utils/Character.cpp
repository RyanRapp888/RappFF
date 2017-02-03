#include "Character.h"
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

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

Character::Character(const std::string &name, CharacterType ctype, CharMotion cmot,
	                 int worldx, int worldy, GameMap *gamemap_ptr):
	   m_name(name),
	   m_chartype(ctype),
	   m_char_motion(cmot),
	   m_worldx(worldx),
	   m_worldy(worldy),
	   m_gamemap_ptr(gamemap_ptr)
{
	SetTileType(CTypeToTileType[ctype]);
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

bool Character::CanOccupyTileType(TileType tt)
{
	if (!permission_initialized) InitTravelPermissions();
	std::vector<TileType> &vv = travel_permissions[m_chartype];
	auto iter = std::find(vv.begin(), vv.end(), tt);
	if (iter != vv.end()) return true;
	return false;
}

void Character::SetLocation(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= m_gamemap_ptr->GetWorldMaxX()) x = m_gamemap_ptr->GetWorldMaxX() - 1;
	if (y < 0) y = 0;
	if (y >= m_gamemap_ptr->GetWorldMaxY()) y = m_gamemap_ptr->GetWorldMaxY() - 1;

	TileType tt;
	m_gamemap_ptr->GetTileType(x, y, tt);

	if (CanOccupyTileType(tt))
	{
		m_worldx = x;
		m_worldy = y;
	}

}

int Character::GetWorldX() { return m_worldx; }

int Character::GetWorldY() { return m_worldy; }
