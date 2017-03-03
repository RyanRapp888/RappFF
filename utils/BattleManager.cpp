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

void BattleManager::ProcessQueue(BattleRoundOutcome &outcome, Loot &loot)
{
	BattleRoundOutcome bro = BattleRoundOutcome::ALL_ENEMIES_DIED;
	loot.m_pooled_money = 999999;
}