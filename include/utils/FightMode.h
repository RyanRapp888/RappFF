#ifndef FIGHTSCREEN_H__
#define FIGHTSCREEN_H__

#include "Character.h"
#include "WindowSection.h"
#include "GameMap.h"
#include "Tile.h"
#include "Text.h"

#define N_FRAMES 3

enum class FightSubMode
{
	PICK_ACTION,       // m_cur_action_idx
	PICK_ITEM,         // m_cur_item_idx
	PICK_ITEM_TARGET,  // m_cur_item_target_idx
	PICK_MONSTER,      // m_cur_monster_idx
	PICK_NOMODE
};

struct BattleEvent
{
	BattleEvent()
	{
		is_actor_hero = true;
		is_aoe = false;
		is_aoe_target_heroes = false;
		actor_idx = -1;
		target_idx = -1;
		action_type_id = -1;
		item_idx = -1;
	}

	BattleEvent(bool iah, bool ia, bool iath, int ai, int ti, int ati, int ii)
	{
		is_actor_hero = iah;
		is_aoe = ia;
		is_aoe_target_heroes = iath;
		actor_idx = ai;
		target_idx = ti;
		action_type_id = ati;
		item_idx = ii;
	}
	
	void SetAsRetreatEvent(int actor_retreating_idx)
	{
		is_actor_hero = true;
		is_aoe = false;
		is_aoe_target_heroes = false;
		actor_idx = actor_retreating_idx;
		target_idx = -1;
		action_type_id = 3;
		item_idx = -1;
	}

	bool is_actor_hero;
	bool is_aoe;
	bool is_aoe_target_heroes;
	int actor_idx;
	int target_idx;
	int action_type_id;
	int item_idx;
};

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
	void DrawMonsterWindow();
	void DrawHeroWindow();
	void DoFightAction();

	void AdvanceToNextHero();

	Tile m_top_tile;
	Tile m_monsterstats_tile;
	Tile m_herostats_tile;
	
	Text *m_texthandler_ptr;
	GLuint m_primary_shaderid;

	bool m_hero_turn;
	int m_hero_turn_idx;
	int m_hero_anim_pct;
	int m_hero_anim_dir;

	int m_monster_turn_idx;
	
	FightSubMode m_sub_mode;
	int m_cur_action_idx;
	int m_cur_monster_idx;
	int m_cur_item_idx;
	int m_cur_item_target_idx;
	bool m_fight_ended;
	std::vector<Character> m_monsters;

	std::vector<BattleEvent> m_battle_queue;
};

#endif
