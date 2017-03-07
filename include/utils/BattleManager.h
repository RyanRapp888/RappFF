#ifndef _BATTLEMANAGER_H__
#define _BATTLEMANAGER_H__

#include <vector>
#include "Character.h"

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
		is_actor_hero = true;
		is_aoe = false;
		is_aoe_target_heroes = false;
		actor_idx = -1;
		target_idx = -1;
		action_type_id = ActionType::NOACTION;
		item_idx = -1;
	}

	BattleEvent(bool iah, bool ia, bool iath, int ai, int ti, ActionType ati, int ii)
	{
		is_actor_hero = iah;
		is_aoe = ia;
		is_aoe_target_heroes = iath;
		actor_idx = ai;
		target_idx = ti;
		action_type_id = ati;
		item_idx = ii;
	}

	void SetAsRetreatEvent(int actor_retreating_idx)
	{
		is_actor_hero = true;
		is_aoe = false;
		is_aoe_target_heroes = false;
		actor_idx = actor_retreating_idx;
		target_idx = -1;
		action_type_id = ActionType::RETREAT;
		item_idx = -1;
	}

	bool is_actor_hero;
	bool is_aoe;
	bool is_aoe_target_heroes;
	int actor_idx;
	int target_idx;
	ActionType action_type_id;
	int item_idx;
};

enum class BattleRoundOutcome
{
	ALL_HEROES_DIED,
	ALL_MOBS_DIED,
	HEROES_RETREATED,
	MOBS_RETREATED,
    BATTLE_ONGOING
};

struct Loot
{
	Loot::Loot():m_pooled_money(0), m_each_xp(0),m_is_empty(true){}
	bool IsEmpty(){ return m_is_empty; }
	int m_pooled_money;
	int m_each_xp;
	
	bool m_is_empty;
	std::vector<Item> m_items;
	std::vector<Spell> m_spell;
	std::vector<Armor> m_armor;
	std::vector<Weapon> m_weapons;
};

class BattleManager
{

public:
	BattleManager();
	std::vector<std::string> GetFightOptions();
	void SetMobs(std::vector<Character> dat);
	void SetHeroes(std::vector<Character *> &dat);
	void AddBattleEvent(BattleEvent be){ m_battle_queue.emplace_back(be); }
	void ClearFightEnded(){ m_fight_ended = false; }
	
	int GetNMonsters() const{ return m_mobs.size(); }
	int GetNItems(int hero_idx){ return m_hero_ptrs[hero_idx]->GetNItems(); }
	int GetNHeroes(){ return m_hero_ptrs.size(); }
	bool IsHeroActive(int hero_idx){ return m_hero_ptrs[hero_idx]->IsActive(); }
	TileType GetMonsterTileType(int monster_id){ return m_mobs[monster_id].GetTileType(); }
	std::string GetMonsterName(int monster_id){ return m_mobs[monster_id].GetName(); }
	void Clear(){ m_mobs.clear(); m_hero_ptrs.clear(); m_battle_queue.clear(); }
	void ProcessQueue(BattleRoundOutcome &outcome, Loot &loot);

private:
	bool m_fight_ended;
	std::vector<Character> m_mobs;
	std::vector<Character *> m_hero_ptrs;
	std::vector<BattleEvent> m_battle_queue;
	void ProcessAttackEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome);
	void ProcessSpecialEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome);
	void ProcessItemEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome);
	void ProcessRetreatEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome);
};

#endif
