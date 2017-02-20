#ifndef FIGHTSCREEN_H__
#define FIGHTSCREEN_H__

#include "Character.h"
#include "WindowSection.h"
#include "GameMap.h"
#include "Tile.h"

class FightScreen : public WindowSection
{
public:

	FightScreen(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct);
	~FightScreen() {}
	void Refresh();

private:
	Tile m_five_window_parts[5];

	

};

#endif
