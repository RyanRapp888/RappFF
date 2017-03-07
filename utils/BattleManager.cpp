#include "BattleManager.h"

std::vector<std::string> BattleManager::GetFightOptions()
{
	std::vector<std::string> results = { "Attack", "Special Move", "Item", "Retreat" };
	return results;
}

BattleManager::BattleManager()
{
	m_fight_ended = false;
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

/*

bool is_actor_hero;
bool is_aoe;
bool is_aoe_target_heroes;
int actor_idx;
int target_idx;
ActionType action_type_id;
int item_idx;
*/

static int get_rand_0_99()
{
	static bool is_seeded(false);
	if (!is_seeded)
	{
		srand(14);
		is_seeded = true;
	}
	int result = rand() % 100;
	return result;
}

void BattleManager::ProcessAttackEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome)
{


}

void BattleManager::ProcessSpecialEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome)
{


}

void BattleManager::ProcessItemEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome)
{


}

void BattleManager::ProcessRetreatEvent(BattleEvent &cur_be, BattleRoundOutcome &outcome)
{
	outcome = BattleRoundOutcome::BATTLE_ONGOING;

	if (cur_be.is_actor_hero)
	{
		// Heroes are attempting to retreat
		Character *cc = m_hero_ptrs[cur_be.actor_idx];
		if (cc->IsActive())
		{
			int flee_val = cc->GetFlee();
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
				outcome = BattleRoundOutcome::HEROES_RETREATED;
				return;
			}
			else
			{
				int diff = flee_val - (static_cast<double>(mobs_sum) / n_active_mobs);
				float pct = (static_cast<double>(diff) / 2.0) + 49.5;
				int rand = get_rand_0_99();
				if (pct >= rand)
				{
					outcome = BattleRoundOutcome::HEROES_RETREATED;
				}
			}
		}
	}
	else
	{
		// Mob is attempting to retreat
		Character &mm = m_mobs[cur_be.actor_idx];
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
				outcome = BattleRoundOutcome::MOBS_RETREATED;
				return;
			}
			else
			{
				int diff = flee_val - (static_cast<double>(heroes_sum) / n_active_heroes);
				float pct = (static_cast<double>(diff) / 2.0) + 49.5;
				int rand = get_rand_0_99();
				if (pct >= rand)
				{
					outcome = BattleRoundOutcome::MOBS_RETREATED;
				}
			}
		}
	}
}

void BattleManager::ProcessQueue(BattleRoundOutcome &outcome, Loot &loot)
{
	int pos(0);
	outcome = BattleRoundOutcome::BATTLE_ONGOING;

	while(!m_fight_ended && pos < m_battle_queue.size())
	{
		BattleEvent &cur_be = m_battle_queue[pos];

		if(cur_be.action_type_id == ActionType::ATTACK)
		{
			ProcessAttackEvent(cur_be, outcome);
		}
		else if (cur_be.action_type_id == ActionType::SPECIAL)
		{
			ProcessSpecialEvent(cur_be, outcome);
		}
		else if (cur_be.action_type_id == ActionType::ITEM)
		{
			ProcessItemEvent(cur_be, outcome);
		}
		else if (cur_be.action_type_id == ActionType::RETREAT)
		{
			ProcessRetreatEvent(cur_be, outcome);
			if (outcome == BattleRoundOutcome::HEROES_RETREATED ||
				outcome == BattleRoundOutcome::MOBS_RETREATED)
			{
				m_fight_ended = true;
				m_battle_queue.clear();
			}
		}
		pos++;
	}
}