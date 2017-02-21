#ifndef FIGHTSCREEN_H__
#define FIGHTSCREEN_H__

#include "Character.h"
#include "WindowSection.h"
#include "GameMap.h"
#include "Tile.h"

class FightMode : public WindowSection
{
public:

	FightMode(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct);
	~FightMode() {}
	void Refresh();
	bool HandleKey(int key, int scancode, int action, int mods);
	void StartFight() { m_fight_ended = false; }
	bool FightEnded() { return m_fight_ended; }

private:
	Tile m_five_window_parts[5];
	bool m_fight_ended;

};

#endif
