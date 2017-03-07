#include <iostream>
#include "FightMode.h"

using namespace std;

void RetreatTest(BattleManager &bm, BattleRoundOutcome &bro, Loot &lt)
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

	bm.ProcessQueue(bro, lt);
	
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

	BattleManager bb;
	std::vector<Character *> hptrs;
	hptrs.resize(4);
	hptrs[0] = &h[0];
	hptrs[1] = &h[1];
	hptrs[2] = &h[2];
	hptrs[3] = &h[3];
	bb.SetHeroes(hptrs);
	bb.SetMobs(gamemap_ptr->GetMonsters(0,0));

	BattleRoundOutcome bro;
	Loot lt;
	RetreatTest(bb, bro, lt);
	if (bro != BattleRoundOutcome::HEROES_RETREATED && lt.IsEmpty())
	{
		std::cout << "Heroes should have been able to retreat" << std::endl;
		return false;
	}
		
	bb.ClearFightEnded();
	hptrs[0]->SetFlee(0);
	hptrs[1]->SetFlee(0);
	hptrs[2]->SetFlee(0);
	hptrs[3]->SetFlee(0);
	RetreatTest(bb, bro, lt);
	if (bro != BattleRoundOutcome::BATTLE_ONGOING)
	{
		std::cout << "Heroes should have not been able to retreat" << std::endl;
		return false;
	}

	return true;
}