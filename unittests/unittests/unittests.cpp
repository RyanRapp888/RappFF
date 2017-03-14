#include <iostream>
#include "FightMode.h"

using namespace std;

void PopulateMobData(std::vector<Character> &mobs, int level)
{
	int n_mobs = 4;
	mobs.resize(n_mobs);

	for (int aa = 0; aa < n_mobs; aa++)
	{
		mobs[aa].SetName("Beflik");
		mobs[aa].SetCharacterType(CharacterType::SKELETON);
		mobs[aa].SetMaxHP(level * 20);
		mobs[aa].SetHP(level * 20);
		mobs[aa].SetLocation(-99, -99);
		mobs[aa].SetFlee(level * 3);
		mobs[aa].SetAccuracy(level * 3);
		mobs[aa].SetDodge(level * 3);
		mobs[aa].SetAttackSkill(level * 3);
		mobs[aa].SetAbsorb(level * 3);
	}
	
}

void PopulateUnfleeableMobData(std::vector<Character> &mobs)
{
	mobs.resize(4);
	mobs[0].SetName("Beflik");
	mobs[0].SetCharacterType(CharacterType::SKELETON);
	//mobs[0].
	mobs[0].SetHP(20);
	mobs[0].SetMaxHP(20);
	mobs[0].SetLocation(-99, -99);
	mobs[0].SetFlee(99);
	mobs[0].SetAccuracy(3);
	mobs[0].SetDodge(3);	

	mobs[1].SetName("Slugpo");
	mobs[1].SetCharacterType(CharacterType::SKELETON);
	mobs[1].SetHP(20);
	mobs[1].SetMaxHP(20);
	mobs[1].SetLocation(-99, -99);
	mobs[1].SetFlee(99);
	mobs[1].SetAccuracy(3);
	mobs[1].SetDodge(3);

	mobs[2].SetName("Gravy-Covered Walrus");
	mobs[2].SetCharacterType(CharacterType::BEAR);
	mobs[2].SetHP(20);
	mobs[2].SetMaxHP(20);
	mobs[2].SetLocation(-99, -99);
	mobs[2].SetFlee(99);
	mobs[2].SetAccuracy(3);
	mobs[2].SetDodge(3);

	mobs[3].SetName("Big Fat Horse");
	mobs[3].SetCharacterType(CharacterType::BEAR);
	mobs[3].SetHP(20);
	mobs[3].SetMaxHP(20);
	mobs[3].SetLocation(-99, -99);
	mobs[3].SetFlee(99);
	mobs[3].SetAccuracy(3);
	mobs[3].SetDodge(3);
}


void RetreatTest(BattleManager &bm, BattleRoundOutcome &bro)
{
	BattleEvent be;
	be.SetAsRetreatEvent(0);
	bm.AddBattleEvent(be);
	be.SetAsRetreatEvent(1);
	bm.AddBattleEvent(be);
	be.SetAsRetreatEvent(2);
	bm.AddBattleEvent(be);
	be.SetAsRetreatEvent(3);
	bm.AddBattleEvent(be);

	bm.ProcessQueue(bro);
	
}

void AttackTest(BattleManager &bm, BattleRoundOutcome &bro)
{
	BattleEvent be_hero_att_mob(true, false, false, 0, 0, ActionType::ATTACK, -1);
	bm.AddBattleEvent(be_hero_att_mob);
	be_hero_att_mob.m_actor_idx = 1;
	be_hero_att_mob.m_target_idx = 1;
	bm.AddBattleEvent(be_hero_att_mob);
	be_hero_att_mob.m_actor_idx = 2;
	be_hero_att_mob.m_target_idx = 2;
	bm.AddBattleEvent(be_hero_att_mob);
	be_hero_att_mob.m_actor_idx = 3;
	be_hero_att_mob.m_target_idx = 3;
	bm.AddBattleEvent(be_hero_att_mob);

	BattleEvent be_mob_att_hero(false, false, false, 0, 0, ActionType::ATTACK, -1);
	bm.AddBattleEvent(be_mob_att_hero);
	be_mob_att_hero.m_actor_idx = 1;
	be_mob_att_hero.m_target_idx = 1;
	bm.AddBattleEvent(be_mob_att_hero);
	be_mob_att_hero.m_actor_idx = 2;
	be_mob_att_hero.m_target_idx = 2;
	bm.AddBattleEvent(be_mob_att_hero);
	be_mob_att_hero.m_actor_idx = 3;
	be_mob_att_hero.m_target_idx = 3;
	bm.AddBattleEvent(be_mob_att_hero);

	bm.ProcessQueue(bro);

}


int main(void)
{
	GameMap *gamemap_ptr = GameMap::GetInstance();

	if (!gamemap_ptr->LoadGameMap(256, 256))
	{
		std::cout << "Error: Could not load game map" << std::endl;
	}

	Character h[4];
	h[0].SetCharacterType(CharacterType::MAINCHAR);
	h[1].SetCharacterType(CharacterType::OCTOPUS);
	h[2].SetCharacterType(CharacterType::PRINCESS);
	h[3].SetCharacterType(CharacterType::JELLYBEAN);

	BattleManager battle_manager;
	std::vector<Character *> hptrs;
	hptrs.resize(4);
	hptrs[0] = &h[0];
	hptrs[1] = &h[1];
	hptrs[2] = &h[2];
	hptrs[3] = &h[3];
	battle_manager.SetHeroes(hptrs);
	battle_manager.SetMobs(gamemap_ptr->GetMonsters(0,0));

	BattleRoundOutcome bro;
	Loot lt;
	RetreatTest(battle_manager, bro);
	if (bro.GetOutcomeType() != OutcomeType::HEROES_RETREATED && lt.IsEmpty())
	{
		std::cout << "Heroes should have been able to retreat" << std::endl;
		return false;
	}
		
	battle_manager.ClearFightEnded();
	hptrs[0]->SetFlee(0);
	hptrs[1]->SetFlee(0);
	hptrs[2]->SetFlee(0);
	hptrs[3]->SetFlee(0);
	std::vector<Character > mobs;
	PopulateUnfleeableMobData(mobs);
	battle_manager.SetMobs(mobs);
	RetreatTest(battle_manager, bro);
	if (bro.GetOutcomeType() != OutcomeType::BATTLE_ONGOING)
	{
		std::cout << "Heroes should have not been able to retreat" << std::endl;
		return false;
	}

	battle_manager.ClearFightEnded();
	mobs.clear();
	PopulateMobData(mobs, 33);
	battle_manager.SetMobs(mobs);
	while (bro.GetOutcomeType() == OutcomeType::BATTLE_ONGOING)
	{
		AttackTest(battle_manager, bro);
	}


	return true;
}