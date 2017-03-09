#include <iostream>
#include "FightMode.h"

using namespace std;

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
	BattleEvent be_single_monster(true, false, false, 0, 0, ActionType::ATTACK, -1);
	bm.AddBattleEvent(be_single_monster);
	be_single_monster.actor_idx = 1;
	be_single_monster.target_idx = 1;
	bm.AddBattleEvent(be_single_monster);
	be_single_monster.actor_idx = 2;
	be_single_monster.target_idx = 2;
	bm.AddBattleEvent(be_single_monster);
	be_single_monster.actor_idx = 3;
	be_single_monster.target_idx = 3;
	bm.AddBattleEvent(be_single_monster);

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
	RetreatTest(battle_manager, bro);
	if (bro.GetOutcomeType() != OutcomeType::BATTLE_ONGOING)
	{
		std::cout << "Heroes should have not been able to retreat" << std::endl;
		return false;
	}

	battle_manager.ClearFightEnded();
	AttackTest(battle_manager, bro);


	return true;
}