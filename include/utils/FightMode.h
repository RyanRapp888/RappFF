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
	void StartFight();
	bool FightEnded() { return m_fight_ended; }
	
private:

	void DrawTopWindow();
	void DrawMonsterStatsWindow();
	void DrawHeroStatsWindow();

	Tile m_top_tile;
	Tile m_monsterstats_tile;
	Tile m_herostats_tile;
	bool m_fight_ended;
	Text *m_texthandler_ptr;
	GLuint m_primary_shaderid;
	std::vector<Character> m_monsters;

};

#endif
