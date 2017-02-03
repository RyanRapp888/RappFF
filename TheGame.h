#ifndef THEGAME_H__
#define THEGAME_H__

#include "GameMap.h"
#include "Character.h"
#include "Display.h"

class TheGame
{
public:
	TheGame() {}
	void KeyHandler(int key, int scancode, int action, int mods);
	void Play();
	~TheGame()
	{
		delete m_display_ptr; m_display_ptr = nullptr;
		delete m_gamemap_ptr; m_gamemap_ptr = nullptr;
	}

private:

	GameMap *m_gamemap_ptr;
	Character *m_mainchar_ptr;
	Display *m_display_ptr;
};

#endif
