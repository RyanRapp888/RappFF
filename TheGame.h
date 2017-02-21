#ifndef THEGAME_H__
#define THEGAME_H__

#include "GameMap.h"
#include "Character.h"
#include "Display.h"
#include "FightMode.h"
#include "TiledGameBoard.h"

enum class GameMode
{
	MapWalkingMode,
	FightMode
};

class TheGame
{
public:
	TheGame() :m_display_ptr(nullptr), m_cur_mode(GameMode::MapWalkingMode),m_mapwalkingmode_ws(nullptr),m_fightmode_ws(nullptr) {}
	void KeyHandler(int key, int scancode, int action, int mods);
	void Play();
	~TheGame()
	{
		//clean all the stuff
	}

private:
	void SetCurMode(GameMode dat);
	void Interact(int x, int y);
	Character m_mainchar;
	Display *m_display_ptr;
	TiledGameBoard *m_mapwalkingmode_ws;
	FightMode *m_fightmode_ws;
	GameMode m_cur_mode;
	int m_xrot, m_yrot, m_zrot;
};

#endif
