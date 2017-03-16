#ifndef _BATTLEMANAGER_H__
#define _BATTLEMANAGER_H__

#include <vector>
#include "Character.h"
#include <sstream>

enum class ActionType
{
	NOACTION = -1,
	ATTACK = 0,
	SPECIAL = 1,
	ITEM = 2,
	RETREAT = 3
};

struct BattleEvent
{
	BattleEvent()
	{
		m_is_actor_hero = true;
		m_is_aoe = false;
		m_is_aoe_target_heroes = false;
		m_actor_idx = -1;
		m_target_idx = -1;
		m_action_type_id = ActionType::NOACTION;
		m_item_idx = -1;
	}

	BattleEvent(bool is_actor_hero, bool is_aoe, bool is_aoe_target_heroes, int actor_idx, int target_idx, ActionType action_type_id, int itemidx)
	{
		m_is_actor_hero = is_actor_hero;
		m_is_aoe = is_aoe;
		m_is_aoe_target_heroes = is_aoe_target_heroes;
		m_actor_idx = actor_idx;
		m_target_idx = target_idx;
		m_action_type_id = action_type_id;
		m_item_idx = itemidx;
	}

	void SetAsRetreatEvent(int actor_retreating_idx)
	{
		m_is_actor_hero = true;
		m_is_aoe = false;
		m_is_aoe_target_heroes = false;
		m_actor_idx = actor_retreating_idx;
		m_target_idx = -1;
		m_action_type_id = ActionType::RETREAT;
		m_item_idx = -1;
	}

	bool m_is_actor_hero;
	bool m_is_aoe;
	bool m_is_aoe_target_heroes;
	int m_actor_idx;
	int m_target_idx;
	ActionType m_action_type_id;
	int m_item_idx;
};

enum class OutcomeType
{
	ALL_HEROES_DIED,
	ALL_MOBS_DIED,
	HEROES_RETREATED,
	MOBS_RETREATED,
    BATTLE_ONGOING
};

struct Loot
{
	Loot::Loot():m_pooled_money(0), m_each_xp(0){}
	std::string GetLootString() const
	{
		std::string result;
		std::ostringstream rstream;
		rstream << "m_pooled_money = " << m_pooled_money << std::endl;
		rstream << "m_each_xp = " << m_each_xp << std::endl;
		for (const auto item : m_items)
		{
			rstream << "m_item[] -> " << item.GetName() << std::endl;
		}
		for (const auto spell : m_spells)
		{
			rstream << "m_spell[] -> " << spell.GetName() << std::endl;
		}
		for (const auto armor : m_armors)
		{
			rstream << "m_armor[] -> " << armor.GetName() << std::endl;
		}
		for (const auto weapon : m_weapons)
		{
			rstream << "m_weapon[] -> " << weapon.GetName() << std::endl;
		}
		result = rstream.str();
		return result;
	}
	int m_pooled_money;
	int m_each_xp;
	
	std::vector<Item> m_items;
	std::vector<Spell> m_spells;
	std::vector<Armor> m_armors;
	std::vector<Weapon> m_weapons;
};



class BattleRoundOutcome
{
public:

	BattleRoundOutcome() :m_type(OutcomeType::BATTLE_ONGOING){}
	void SetOutcomeType(OutcomeType dat){ m_type = dat; }
	OutcomeType GetOutcomeType() const{ return m_type; }
	void SetLoot(Loot &dat){ m_loot = dat; }
	Loot GetLoot() const{ return m_loot; }
private:

	Loot m_loot;
	OutcomeType m_type;

};

class BattleManager
{

public:
	BattleManager();
	std::vector<std::string> GetFightOptions();
	void SetMobs(std::vector<Character> dat);
	void SetHeroes(std::vector<Character *> &dat);
	void AddBattleEvent(BattleEvent be){ m_battle_queue.emplace_back(be); }
	void ClearFightEnded(){ m_battle_queue.clear(); m_fight_ended = false; m_mobs.clear(); }
	bool LookForActiveMob(int start_id, int look_dir, int &found_id);
	bool LookForActiveHero(int start_id, int look_dir, int &found_id);
	bool LookForAliveMob(int start_id, int look_dir, int &found_id);
	bool LookForAliveHero(int start_id, int look_dir, int &found_id);

	Character *GetMobPtr(int mob_id){return &m_mobs[mob_id];}
	int GetNMobs() const{ return m_mobs.size(); }
	int GetNItems(int hero_idx){ return m_hero_ptrs[hero_idx]->GetNItems(); }
	int GetNHeroes(){ return m_hero_ptrs.size(); }
	bool IsHeroActive(int hero_idx){ return m_hero_ptrs[hero_idx]->IsActive(); }
	TileType GetMobTileType(int mob_id){ return m_mobs[mob_id].GetTileType(); }
	bool IsMobDead(int mob_id){ return m_mobs[mob_id].IsDead(); }
	std::string GetMobName(int mob_id){ return m_mobs[mob_id].GetName(); }
	void Clear(){ m_mobs.clear(); m_hero_ptrs.clear(); m_battle_queue.clear(); }
	void ProcessQueue(BattleRoundOutcome &outcome);
	void CheckMobParty(BattleRoundOutcome &outcome);
	void CheckHeroParty(BattleRoundOutcome &outcome);
	void CalculateLoot(BattleRoundOutcome &outcome);

	void AddMobOffenseEvents();
	

private:
	bool m_fight_ended;
	std::vector<Character> m_mobs;
	std::vector<Character *> m_hero_ptrs;
	std::vector<BattleEvent> m_battle_queue;
	void ProcessAttackEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome);
	void ProcessSpecialEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome);
	void ProcessItemEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome);
	void ProcessRetreatEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome);
	int CalculateAttackPts(Character *agressor, Character *defender, ArmorType &at);
	bool LookForBase(std::vector<Character *> actorlist, int start_id, int look_dir, int &found_id);
};

#endif
