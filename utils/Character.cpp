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
	                 int worldx, int worldy):
	   m_name(name),
	   m_chartype(ctype),
	   m_char_motion(cmot),
	   m_worldx(worldx),
	   m_worldy(worldy),
	   m_gamemap_ptr(nullptr)
{
	SetTileType(CTypeToTileType[ctype]);
}

void Character::SetGameMapPtr(GameMap *gm_ptr)
{
	m_gamemap_ptr = gm_ptr;
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
	m_gamemap_ptr->GetTileType(x, y, typ);
	//First, we see if our character is allowed to be on the given tiletype
	std::vector<TileType> &vv = travel_permissions[m_chartype];
	auto iter = std::find(vv.begin(), vv.end(), typ);
	if (iter == vv.end())
	{
		return false;
	}

	//then we make sure no other players are occupying the space
	return !(m_gamemap_ptr->HasACharacter(x, y));
}

void Character::SetLocation(int x, int y)
{
	if (m_gamemap_ptr == nullptr) return;
	if (x < 0) x = 0;
	if (x >= m_gamemap_ptr->GetWorldMaxX()) x = m_gamemap_ptr->GetWorldMaxX() - 1;
	if (y < 0) y = 0;
	if (y >= m_gamemap_ptr->GetWorldMaxY()) y = m_gamemap_ptr->GetWorldMaxY() - 1;

	//CanOccupyLocation looks to see if our character can step on the desired
	//tile type AND makes sure its not occupied.
	if (CanOccupyLocation(x, y))
	{
		m_worldx = x;
		m_worldy = y;
	}

}

Character &Character::operator=(const Character & other)
{
	if (this != &other) // protect against invalid self-assignment
	{
		this->m_chartype = other.m_chartype;
		this->m_name = other.m_name;
		this->m_char_motion = other.m_char_motion;
		if (other.m_gamemap_ptr != nullptr) this->m_gamemap_ptr = other.m_gamemap_ptr;
		this->m_worldx = other.m_worldx;
		this->m_worldy = other.m_worldy;
		DrawnCharacter::operator=(other);

	}

	return *this;
}

int Character::GetX() { return m_worldx; }

int Character::GetY() { return m_worldy; }
