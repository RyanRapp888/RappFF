#ifndef FIGHTSCREEN_H__
#define FIGHTSCREEN_H__

#include "Character.h"
#include "WindowSection.h"
#include "GameMap.h"
#include "Tile.h"
#include "Text.h"

#define N_FRAMES 3

class FightMode : public WindowSection
{
public:

	FightMode(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct);
	bool SetTextHandler(Text *texthandler);
	bool SetPrimaryShader(GLuint primary_shader);
	~FightMode() {}
	void Refresh();
	bool HandleKey(int key, int scancode, int action, int mods);
	void StartFight() { m_fight_ended = false; }
	bool FightEnded() { return m_fight_ended; }


private:
	Tile m_five_window_parts[N_FRAMES];
	bool m_fight_ended;
	Text *m_texthandler_ptr;
	GLuint m_primary_shaderid;

};

#endif
