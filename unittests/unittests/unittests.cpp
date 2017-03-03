#include <iostream>
#include "FightMode.h"

using namespace std;

bool EasyRetreatTest(BattleManager &bm)
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

	BattleRoundOutcome bro;
	Loot lt;
	bm.ProcessQueue(bro, lt);
	if (bro == BattleRoundOutcome::HEROES_RETREATED && lt.IsEmpty())
	{
		return true;
	}
	return false;
}

int main(void)
{
	int abc;
	std::cin >> abc;

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

	if (!EasyRetreatTest(bb))
	{
		std::cout << "Failed EasyRetreatTest" << std::endl;
		int abc;
		std::cin >> abc;
		return false;
	}
	

	return true;
}