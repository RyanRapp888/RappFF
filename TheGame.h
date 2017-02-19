#ifndef THEGAME_H__
#define THEGAME_H__

#include "GameMap.h"
#include "Character.h"
#include "Display.h"

class TheGame
{
public:
	TheGame() :m_display_ptr(nullptr), m_fightymode(false) {}
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
	bool m_fightymode;
	int m_xrot, m_yrot, m_zrot;
};

#endif
