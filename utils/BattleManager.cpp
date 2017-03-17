#include "BattleManager.h"
#include "RandUtils.h"


struct level_stats_entry
{
	level_stats_entry() :low_dmg(0), hi_dmg(0){}
	level_stats_entry(int lo, int hi) :low_dmg(lo), hi_dmg(hi){}
	int low_dmg;
	int hi_dmg;

};

static level_stats_entry &GetLevelAttributes(int level)
{
	static std::map<int, level_stats_entry> m_levelinfo;
	static level_stats_entry bad_level(0, 0);

	if (m_levelinfo.size() == 0)
	{
		m_levelinfo[1] = level_stats_entry(5.00, 8);
		m_levelinfo[2] = level_stats_entry(5.33, 8);
		m_levelinfo[3] = level_stats_entry(6.66, 10);
		m_levelinfo[4] = level_stats_entry(8.33, 12.5);
		m_levelinfo[5] = level_stats_entry(13.33, 20);
		m_levelinfo[6] = level_stats_entry(20.00, 30);
		m_levelinfo[7] = level_stats_entry(26.66, 40);
		m_levelinfo[8] = level_stats_entry(33.33, 50);
		m_levelinfo[9] = level_stats_entry(40.00, 60);
		m_levelinfo[10] = level_stats_entry(46.66, 70);
		m_levelinfo[11] = level_stats_entry(53.33, 80);
		m_levelinfo[12] = level_stats_entry(61.66, 92.5);
		m_levelinfo[13] = level_stats_entry(70.00, 105);
		m_levelinfo[14] = level_stats_entry(78.33, 117.5);
		m_levelinfo[15] = level_stats_entry(86.66, 130);
		m_levelinfo[16] = level_stats_entry(96.66, 145);
		m_levelinfo[17] = level_stats_entry(106.66, 160);
		m_levelinfo[18] = level_stats_entry(116.66, 175);
		m_levelinfo[19] = level_stats_entry(126.66, 190);
		m_levelinfo[20] = level_stats_entry(136.66, 205);
		m_levelinfo[21] = level_stats_entry(148.33, 222.5);
		m_levelinfo[22] = level_stats_entry(160.00, 240);
		m_levelinfo[23] = level_stats_entry(171.66, 257.5);
		m_levelinfo[24] = level_stats_entry(183.33, 275);
		m_levelinfo[25] = level_stats_entry(196.66, 295);
		m_levelinfo[26] = level_stats_entry(210.00, 315);
		m_levelinfo[27] = level_stats_entry(223.33, 335);
		m_levelinfo[28] = level_stats_entry(236.66, 355);
		m_levelinfo[29] = level_stats_entry(250.00, 375);
		m_levelinfo[30] = level_stats_entry(266.66, 400);
		m_levelinfo[31] = level_stats_entry(283.33, 425);
		m_levelinfo[32] = level_stats_entry(300.00, 450);
		m_levelinfo[33] = level_stats_entry(316.66, 475);
	}

	std::map<int, level_stats_entry>::iterator iter = m_levelinfo.find(level);
	if (iter != m_levelinfo.end())
	{
		return iter->second;
	}
	else if (level <= 0)
	{
		return m_levelinfo[1];
	}
	else if (level > 33)
	{
		return m_levelinfo[33];
	}
	return bad_level;
}

BattleManager::BattleManager()
{
	m_fight_ended = false;
}

std::vector<std::string> BattleManager::GetFightOptions()
{
	std::vector<std::string> results = { "Attack", "Special Move", "Item", "Retreat" };
	return results;
}

void BattleManager::SetMobs(std::vector<Character> dat)
{
	m_mobs = dat;
}

void BattleManager::SetHeroes(std::vector<Character *> &dat)
{
	for (int aa = 0; aa < dat.size(); aa++)
	{
		m_hero_ptrs.push_back(dat[aa]);
	}
}

void BattleManager::AddBattleEvent(BattleEvent be)
{ 
	m_battle_queue.emplace_back(be); 
}

void BattleManager::ClearFightEnded()
{ 
	m_battle_queue.clear();
	m_fight_ended = false;
	m_mobs.clear(); 
}

bool(*func_ptr_action)(Character *);

static bool func_active(Character *dat)
{
	return dat->IsActive();
}

static bool func_notdead(Character *dat)
{
	return !dat->IsDead();
}

// look_dir should be +1 or -1
// if a matching character was found, then found_id contains the index and return var == true;
bool BattleManager::LookForActiveMob(int start_id, int look_dir, int &found_id)
{
	func_ptr_action = func_active;
	std::vector<Character *> moblist;
	moblist.resize(m_mobs.size());
	for (int aa = 0; aa < m_mobs.size(); aa++)
	{
		moblist[aa] = &(m_mobs[aa]);
	}

	return LookForBase(moblist, start_id, look_dir, found_id);
}

bool BattleManager::LookForActiveHero(int start_id, int look_dir, int &found_id)
{
	func_ptr_action = func_active;
	return LookForBase(m_hero_ptrs, start_id, look_dir, found_id);
}

bool BattleManager::LookForAliveMob(int start_id, int look_dir, int &found_id)
{
	func_ptr_action = func_notdead;
	std::vector<Character *> moblist;
	moblist.resize(m_mobs.size());
	for (int aa = 0; aa < m_mobs.size(); aa++)
	{
		moblist[aa] = &(m_mobs[aa]);
	}

	return LookForBase(moblist, start_id, look_dir, found_id);
}

bool BattleManager::LookForAliveHero(int start_id, int look_dir, int &found_id)
{
	func_ptr_action = func_notdead;
	return LookForBase(m_hero_ptrs, start_id, look_dir, found_id);
}

Character *BattleManager::GetMobPtr(int mob_id){ return &m_mobs[mob_id]; }
int BattleManager::GetNMobs() const{ return m_mobs.size(); }
int BattleManager::GetNItems(int hero_idx){ return m_hero_ptrs[hero_idx]->GetNItems(); }
int BattleManager::GetNHeroes(){ return m_hero_ptrs.size(); }
bool BattleManager::IsHeroActive(int hero_idx){ return m_hero_ptrs[hero_idx]->IsActive(); }
TileType BattleManager::GetMobTileType(int mob_id){ return m_mobs[mob_id].GetTileType(); }
bool BattleManager::IsMobDead(int mob_id){ return m_mobs[mob_id].IsDead(); }
std::string BattleManager::GetMobName(int mob_id){ return m_mobs[mob_id].GetName(); }
void BattleManager::Clear(){ m_mobs.clear(); m_hero_ptrs.clear(); m_battle_queue.clear(); }
void BattleManager::ProcessQueue(BattleRoundOutcome &outcome)
{
	int pos(0);
	outcome.SetOutcomeType(OutcomeType::BATTLE_ONGOING);

	while (!m_fight_ended && pos < m_battle_queue.size())
	{
		BattleEvent &cur_be = m_battle_queue[pos];

		if (cur_be.m_action_type_id == ActionType::ATTACK)
		{
			ProcessAttackEvent(cur_be, outcome);
		}
		else if (cur_be.m_action_type_id == ActionType::SPECIAL)
		{
			ProcessSpecialEvent(cur_be, outcome);
		}
		else if (cur_be.m_action_type_id == ActionType::ITEM)
		{
			ProcessItemEvent(cur_be, outcome);
		}
		else if (cur_be.m_action_type_id == ActionType::RETREAT)
		{
			ProcessRetreatEvent(cur_be, outcome);
		}

		if (outcome.GetOutcomeType() == OutcomeType::ALL_MOBS_DIED)
		{
			CalculateLoot(outcome);
			m_fight_ended = true;
		}
		else if (outcome.GetOutcomeType() == OutcomeType::ALL_HEROES_DIED)
		{
			m_fight_ended = true;
		}
		else if (outcome.GetOutcomeType() == OutcomeType::HEROES_RETREATED ||
			outcome.GetOutcomeType() == OutcomeType::MOBS_RETREATED)
		{
			m_fight_ended = true;
		}

		pos++;
	}

	m_battle_queue.clear();
}

void BattleManager::CheckMobParty(BattleRoundOutcome &outcome)
{
	bool all_mobs_dead(true);
	for (int mm = 0; mm < m_mobs.size(); mm++)
	{
		if (!m_mobs[mm].IsDead())
		{
			all_mobs_dead = false;
			break;
		}
	}

	if (all_mobs_dead)
	{
		outcome.SetOutcomeType(OutcomeType::ALL_MOBS_DIED);

	}
}

void BattleManager::CheckHeroParty(BattleRoundOutcome &outcome)
{
	bool all_heros_dead(true);
	for (int hh = 0; hh < m_hero_ptrs.size(); hh++)
	{
		if (!m_hero_ptrs[hh]->IsDead())
		{
			all_heros_dead = false;
			break;
		}
	}

	if (all_heros_dead)
	{
		outcome.SetOutcomeType(OutcomeType::ALL_HEROES_DIED);
	}
}

void BattleManager::CalculateLoot(BattleRoundOutcome &outcome)
{
	Loot testloot;
	testloot.m_pooled_money = 50;
	testloot.m_items.push_back(Item("Pot of Dust", UseType::FRIENDLY_AOE));
	testloot.m_weapons.push_back(Weapon("Hickory Switch"));
	outcome.SetLoot(testloot);
}

void BattleManager::AddMobOffenseEvents()
{
	std::vector<int> alive_heroes;

	for (int aa = 0; aa < m_hero_ptrs.size(); aa++)
	{
		if (!m_hero_ptrs[aa]->IsDead())
		{
			alive_heroes.push_back(aa);
		}
	}

	if (alive_heroes.size() == 0) return;

	for (int mm = 0; mm < m_mobs.size(); mm++)
	{
		if (m_mobs[mm].IsActive())
		{
			int m_hero_to_attack = get_rand_0_x(alive_heroes.size() - 1);
			BattleEvent attack_event(false, false, false, mm, alive_heroes[m_hero_to_attack], ActionType::ATTACK, -1);
			m_battle_queue.emplace_back(attack_event);
		}
	}

}

void BattleManager::ProcessAttackEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome)
{
	bool is_hit(false);
	bool is_crit(false);

	if (cur_be.m_is_actor_hero)
	{
		if (cur_be.m_actor_idx < 0 || cur_be.m_actor_idx >= m_hero_ptrs.size())
		{
			std::cout << "Error (ProcessAttackEvent-Hero) invalid index into hero list in BattleManager";
			outcome.SetOutcomeType(OutcomeType::HEROES_RETREATED);
			return;
		}
		Character *hero = m_hero_ptrs[cur_be.m_actor_idx];
		ArmorType at;
		if (hero->IsActive())
		{
			int hero_acc = hero->GetAccuracy();
			int hero_w_acc = hero->GetWeaponAccuracy();
			if (cur_be.m_target_idx < 0 || cur_be.m_target_idx >= m_mobs.size())
			{
				outcome.SetOutcomeType(OutcomeType::MOBS_RETREATED);
				std::cout << "Error (ProcessAttackEvent-Hero): invalid index into mob list in BattleManager\n";
				return;
			}

			int alive_target_idx = cur_be.m_target_idx;
			bool is_found = LookForAliveMob(cur_be.m_target_idx, 1, alive_target_idx);
			if (!is_found)
			{
				// this should never happen because we check if all are dead after each hit.
				return;
			}

			Character *mob = &(m_mobs[alive_target_idx]);

			int armtyp = rand() % 4;
			at = static_cast<ArmorType>(armtyp);
			if (!mob->IsActive()) at = ArmorType::INACTIVE;
			int mob_dodge = mob->GetDodge();
			int mob_armor_dodge = mob->GetArmorDodge(at);
			int hero_avg = (double)(hero_acc + hero_w_acc) / 2.0;
			int mob_avg = (double)(mob_dodge + mob_armor_dodge) / 2.0;
			int diff = hero_avg - mob_avg;
			float pct = ((double)diff / 2.0) + 49.5;
			int rand = get_rand_0_99();
			if (pct >= rand || !mob->IsActive())
			{
				is_hit = true;
				if (rand < 3 || !mob->IsActive()) //critval
				{
					is_crit = true;
				}
			}

			if (is_hit)
			{
				int pts = CalculateAttackPts(hero, mob, at);
				mob->ModifyHP(-1 * pts);
				CheckMobParty(outcome);
			}
		}
	}
	else
	{
		// An enemy is attacking a hero
		if (cur_be.m_actor_idx < 0 || cur_be.m_actor_idx >= m_hero_ptrs.size())
		{
			std::cout << "Error (ProcessAttackEvent-Mob) invalid index into mob list in BattleManager";
			outcome.SetOutcomeType(OutcomeType::MOBS_RETREATED);
			return;
		}
		Character *mob = &(m_mobs[cur_be.m_actor_idx]);
		ArmorType at;
		if (mob->IsActive())
		{
			int mob_acc = mob->GetAccuracy();
			int mob_w_acc = mob->GetWeaponAccuracy();
			if (cur_be.m_target_idx < 0 || cur_be.m_target_idx >= m_hero_ptrs.size())
			{
				std::cout << "Error (ProcessAttackEvent-Mob) invalid index into mob list in BattleManager";
				outcome.SetOutcomeType(OutcomeType::MOBS_RETREATED);
				return;
			}

			Character *hero = m_hero_ptrs[cur_be.m_target_idx];
			if (hero->IsDead())
			{
				for (int aa = 0; aa < m_hero_ptrs.size(); aa++)
				{
					hero = m_hero_ptrs[aa];
					if (!hero->IsDead()) break;
				}
			}

			int armtyp = rand() % 4;
			at = static_cast<ArmorType>(armtyp);
			int hero_dodge = hero->GetDodge();
			int hero_armor_dodge = hero->GetArmorDodge(at);
			int mob_avg = (double)(mob_acc + mob_w_acc) / 2.0;
			int hero_avg = (double)(hero_dodge + hero_armor_dodge) / 2.0;
			int diff = mob_avg - hero_avg;
			float pct = ((double)diff / 2.0) + 49.5;
			int rand = get_rand_0_99();
			if (pct >= rand || !hero->IsActive())
			{
				is_hit = true;
				if (rand < 3 || !hero->IsActive()) //critval
				{
					is_crit = true;
				}
			}

			if (is_hit)
			{
				int pts = CalculateAttackPts(mob, hero, at);
				hero->ModifyHP(-1 * pts);
				CheckHeroParty(outcome);
			}
		}
	}

}


void BattleManager::ProcessSpecialEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome)
{
	bool is_hit(false);
	bool is_crit(false);

	if (cur_be.m_is_actor_hero)
	{
		if (cur_be.m_actor_idx < 0 || cur_be.m_actor_idx >= m_hero_ptrs.size())
		{
			std::cout << "Error (ProcessAttackEvent-Hero) invalid index into hero list in BattleManager";
			outcome.SetOutcomeType(OutcomeType::HEROES_RETREATED);
			return;
		}
		Character *hero = m_hero_ptrs[cur_be.m_actor_idx];
		ArmorType at;
		if (hero->IsActive())
		{
			int hero_acc = hero->GetAccuracy();
			int hero_w_acc = hero->GetWeaponAccuracy();
			if (cur_be.m_target_idx < 0 || cur_be.m_target_idx >= m_mobs.size())
			{
				outcome.SetOutcomeType(OutcomeType::MOBS_RETREATED);
				std::cout << "Error (ProcessAttackEvent-Hero): invalid index into mob list in BattleManager\n";
				return;
			}

			int alive_target_idx = cur_be.m_target_idx;
			bool is_found = LookForAliveMob(cur_be.m_target_idx, 1, alive_target_idx);
			if (!is_found)
			{
				// this should never happen because we check if all are dead after each hit.
				return;
			}

			Character *mob = &(m_mobs[alive_target_idx]);

			int armtyp = rand() % 4;
			at = static_cast<ArmorType>(armtyp);
			if (!mob->IsActive()) at = ArmorType::INACTIVE;
			int mob_dodge = mob->GetDodge();
			int mob_armor_dodge = mob->GetArmorDodge(at);
			int hero_avg = (double)(hero_acc + hero_w_acc) / 2.0;
			int mob_avg = (double)(mob_dodge + mob_armor_dodge) / 2.0;
			int diff = hero_avg - mob_avg;
			float pct = ((double)diff / 2.0) + 49.5;
			int rand = get_rand_0_99();
			if (pct >= rand || !mob->IsActive())
			{
				is_hit = true;
				if (rand < 3 || !mob->IsActive()) //critval
				{
					is_crit = true;
				}
			}

			if (is_hit)
			{
				int pts = CalculateAttackPts(hero, mob, at);
				pts *= 2;
				mob->ModifyHP(-1 * pts);
				CheckMobParty(outcome);
			}
		}
	}
	else
	{
		// An enemy is attacking a hero
		if (cur_be.m_actor_idx < 0 || cur_be.m_actor_idx >= m_hero_ptrs.size())
		{
			std::cout << "Error (ProcessAttackEvent-Mob) invalid index into mob list in BattleManager";
			outcome.SetOutcomeType(OutcomeType::MOBS_RETREATED);
			return;
		}
		Character *mob = &(m_mobs[cur_be.m_actor_idx]);
		ArmorType at;
		if (mob->IsActive())
		{
			int mob_acc = mob->GetAccuracy();
			int mob_w_acc = mob->GetWeaponAccuracy();
			if (cur_be.m_target_idx < 0 || cur_be.m_target_idx >= m_hero_ptrs.size())
			{
				std::cout << "Error (ProcessAttackEvent-Mob) invalid index into mob list in BattleManager";
				outcome.SetOutcomeType(OutcomeType::MOBS_RETREATED);
				return;
			}

			Character *hero = m_hero_ptrs[cur_be.m_target_idx];
			if (hero->IsDead())
			{
				for (int aa = 0; aa < m_hero_ptrs.size(); aa++)
				{
					hero = m_hero_ptrs[aa];
					if (!hero->IsDead()) break;
				}
			}

			int armtyp = rand() % 4;
			at = static_cast<ArmorType>(armtyp);
			int hero_dodge = hero->GetDodge();
			int hero_armor_dodge = hero->GetArmorDodge(at);
			int mob_avg = (double)(mob_acc + mob_w_acc) / 2.0;
			int hero_avg = (double)(hero_dodge + hero_armor_dodge) / 2.0;
			int diff = mob_avg - hero_avg;
			float pct = ((double)diff / 2.0) + 49.5;
			int rand = get_rand_0_99();
			if (pct >= rand || !hero->IsActive())
			{
				is_hit = true;
				if (rand < 3 || !hero->IsActive()) //critval
				{
					is_crit = true;
				}
			}

			if (is_hit)
			{
				int pts = CalculateAttackPts(mob, hero, at);
				pts *= 2;
				hero->ModifyHP(-1 * pts);
				CheckHeroParty(outcome);
			}
		}
	}
}

void BattleManager::ProcessItemEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome)
{
	if (cur_be.m_actor_idx < 0 || cur_be.m_actor_idx >= m_hero_ptrs.size())
	{
		std::cout << "Error (ProcessItemEvent-Hero) invalid index into hero list in BattleManager";
		outcome.SetOutcomeType(OutcomeType::HEROES_RETREATED);
		return;
	}

	Character *hero = m_hero_ptrs[cur_be.m_actor_idx];
	if (hero->IsActive())
	{
		const Item &hero_item = hero->GetItemRef(cur_be.m_item_idx);

		if (hero_item.GetType() == UseType::VS_SINGLE)
		{
			int alive_target_idx = cur_be.m_target_idx;
			bool is_found = LookForAliveMob(cur_be.m_target_idx, 1, alive_target_idx);
			if (!is_found)
			{
				// this should never happen because we check if all are dead after each hit.
				return;
			}

			Character *mob = &(m_mobs[alive_target_idx]);
			ArmorType at = ArmorType::BODY;
			int pts = CalculateAttackPts(hero, mob, at);
			pts *= 1.5;
			mob->ModifyHP(-1 * pts);
			CheckMobParty(outcome);
		}
		else if (hero_item.GetType() == UseType::VS_AOE)
		{
			for (int mm = 0; mm < m_mobs.size(); mm++)
			{
				if (!m_mobs[mm].IsDead())
				{
					ArmorType at = ArmorType::BODY;
					int pts = CalculateAttackPts(hero, &m_mobs[mm], at);
					pts *= 1.5;
					m_mobs[mm].ModifyHP(-1 * pts);
					CheckMobParty(outcome);
				}
			}
		}
		else if (hero_item.GetType() == UseType::FRIENDLY_SINGLE)
		{
			int alive_target_idx = cur_be.m_target_idx;
			bool is_found = LookForAliveHero(cur_be.m_target_idx, 1, alive_target_idx);
			if (!is_found)
			{
				// this should never happen because we check if all are dead after each hit.
				return;
			}

			Character *item_friendly_target = m_hero_ptrs[alive_target_idx];
			ArmorType at = ArmorType::BODY;
			int pts = CalculateAttackPts(hero, item_friendly_target, at);
			pts *= 1.5;
			item_friendly_target->ModifyHP(pts);
		}
		else if (hero_item.GetType() == UseType::FRIENDLY_AOE)
		{
			for (int mm = 0; mm < m_hero_ptrs.size(); mm++)
			{
				if (!m_hero_ptrs[mm]->IsDead())
				{
					ArmorType at = ArmorType::BODY;
					int pts = CalculateAttackPts(hero, m_hero_ptrs[mm], at);
					pts *= 1.5;
					m_mobs[mm].ModifyHP(pts);
				}
			}
		}
	}
}

void BattleManager::ProcessRetreatEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome)
{
	outcome.SetOutcomeType(OutcomeType::BATTLE_ONGOING);

	if (cur_be.m_is_actor_hero)
	{
		// Heroes are attempting to retreat
		Character *hero = m_hero_ptrs[cur_be.m_actor_idx];
		if (hero->IsActive())
		{
			int flee_val = hero->GetFlee();
			int mobs_sum(0);
			int n_active_mobs(0);

			for (int aa = 0; aa < m_mobs.size(); aa++)
			{
				if (m_mobs[aa].IsActive())
				{
					mobs_sum += m_mobs[aa].GetFlee();
					n_active_mobs++;
				}
			}

			if (n_active_mobs == 0)
			{
				// This should never happen
				outcome.SetOutcomeType(OutcomeType::HEROES_RETREATED);
				return;
			}
			else
			{
				int diff = flee_val - (static_cast<double>(mobs_sum) / n_active_mobs);
				float pct = (static_cast<double>(diff) / 2.0) + 49.5;
				int rand = get_rand_0_99();
				if (pct >= rand)
				{
					outcome.SetOutcomeType(OutcomeType::HEROES_RETREATED);
				}
			}
		}
	}
	else
	{
		// Mob is attempting to retreat
		Character &mm = m_mobs[cur_be.m_actor_idx];
		if (mm.IsActive())
		{
			int flee_val = mm.GetFlee();
			int heroes_sum(0);
			int n_active_heroes(0);

			for (int aa = 0; aa < m_hero_ptrs.size(); aa++)
			{
				if (m_mobs[aa].IsActive())
				{
					heroes_sum += m_mobs[aa].GetFlee();
					n_active_heroes++;
				}
			}

			if (n_active_heroes == 0)
			{
				outcome.SetOutcomeType(OutcomeType::MOBS_RETREATED);
				return;
			}
			else
			{
				int diff = flee_val - (static_cast<double>(heroes_sum) / n_active_heroes);
				float pct = (static_cast<double>(diff) / 2.0) + 49.5;
				int rand = get_rand_0_99();
				if (pct >= rand)
				{
					outcome.SetOutcomeType(OutcomeType::MOBS_RETREATED);
				}
			}
		}
	}
}

int BattleManager::CalculateAttackPts(Character *agressor, Character *defender, ArmorType &at)
{
	int results(0);
	int ag_attack_skill = agressor->GetAttackSkill();
	int ag_weapon_dmg_rating = agressor->GetWeaponDamage();
	int attack_avg = static_cast<double>(ag_attack_skill + ag_weapon_dmg_rating) / 2.0;
	int ag_functional_level = attack_avg / 3;
	level_stats_entry ag_lse = GetLevelAttributes(ag_functional_level);
	int deltadmg = ag_lse.hi_dmg - ag_lse.low_dmg;

	int def_absorb = defender->GetAbsorb();
	int def_armor_absorb = defender->GetArmorAbsorb(at);
	int def_avg = static_cast<double>(def_absorb + def_armor_absorb) / 2.0;
	int def_functional_level = def_avg / 3;
	level_stats_entry def_lse = GetLevelAttributes(def_functional_level);

	int diff = attack_avg - def_avg;
	float pct = ((double)diff / 2.0) + 49.5;
	int rand = get_rand_0_99();
	if (pct >= rand)
	{
		//ADVANTAGE AGRESSOR!
		float bonus = deltadmg * (pct / 100.0);
		results = ag_lse.low_dmg + bonus;
	}
	else
	{
		results = ag_lse.low_dmg;
		if (rand > 95)
		{
			results -= (deltadmg * .25);
		}
		// DEFENDER GETS DECENT ABSORB
	}
	return results;
}

bool BattleManager::LookForBase(std::vector<Character *> actorlist, int start_id, int look_dir, int &found_id)
{
	int id = start_id;
	int ntries = 0;
	bool found(false);

	if (id < 0)
	{
		id = actorlist.size() - 1;
	}
	else if (id >= actorlist.size())
	{
		id = 0;
	}

	if (actorlist.size() == 0) return false;

	while (1)
	{
		found = func_ptr_action(actorlist[id]);
		if (found)
		{
			found_id = id;
			return true;
		}

		ntries++;
		if (ntries >= actorlist.size())
		{
			found_id = -1;
			return false;
		}

		id += look_dir;
		if (id < 0)
		{
			id = actorlist.size() - 1;
		}
		else if (id >= actorlist.size())
		{
			id = 0;
		}
	}
}