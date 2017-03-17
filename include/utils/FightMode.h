#ifndef FIGHTSCREEN_H__
#define FIGHTSCREEN_H__

#include "Character.h"
#include "WindowSection.h"
#include "GameMap.h"
#include "Tile.h"
#include "Text.h"
#include "BattleManager.h"

#define N_FRAMES 3

enum class FightPickMode
{
	PICK_ACTION,       // m_cur_action_idx
	PICK_ITEM,         // m_cur_item_idx
	PICK_ITEM_TARGET,  // m_cur_item_target_idx
	PICK_MOB,      // m_cur_mob_idx
	PICK_NOMODE
};

class FightMode : public WindowSection
{
public:
	FightMode(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct);
	~FightMode() {}
	bool SetTextHandler(Text *texthandler);
	bool SetPrimaryShader(GLuint primary_shader);
	void Refresh();
	bool HandleKey(int key, int scancode, int action, int mods);
	void StartFight();
	void StartBattleRound();
	void AdvanceToNextHero();
	bool FightEnded();
		
private:

	void DrawTopWindow();
	void DrawMobWindow();
	void DrawHeroWindow();
	
	int m_hero_turn_idx;
	int m_cur_action_idx;
	int m_cur_mob_idx;
	int m_cur_item_idx;
	int m_cur_item_target_idx;
	bool m_fight_ended;
	int m_hero_anim_pct;
	int m_hero_anim_dir;
	BattleManager m_battle;
	bool m_hero_turn;
	Tile m_top_tile;
	Tile m_mobstats_tile;
	Tile m_herostats_tile;
	FightPickMode m_sub_mode;
	Text *m_texthandler_ptr;
	GLuint m_primary_shaderid;
};

#endif
