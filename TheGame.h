#ifndef THEGAME_H__
#define THEGAME_H__

#include "GameMap.h"
#include "Character.h"
#include "Display.h"

class TheGame
{
public:
	TheGame(){}
	void KeyHandler(int key, int scancode, int action, int mods);
	void Play();
	~TheGame()
	{
		//clean all the stuff
	}

private:
	void Interact(int x, int y);
	Character m_mainchar;
	Display *m_display_ptr;
	GameMap *m_gamemap_ptr;
};

#endif
