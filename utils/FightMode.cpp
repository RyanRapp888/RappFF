#include "FightMode.h"
#include <glad.h>
#include <GLFW/glfw3.h>
#include <sstream>

#define HERO_VERT_SPACING .19f
#define MOB_VERT_SPACING .1f

FightMode::FightMode(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct) :
WindowSection(vpt, origxpct, origypct, w_pct, h_pct), m_texthandler_ptr(nullptr), m_primary_shaderid(0)
{
	m_top_tile.SetWindowSectionPtr(this);
	m_top_tile.SetTileType(TileType::GRASS);
	m_top_tile.SetRelativeLocation(0, .5, 1, .5);

	m_mobstats_tile.SetTileType(TileType::FRAME);
	m_mobstats_tile.SetWindowSectionPtr(this);
	m_mobstats_tile.SetRelativeLocation(0, 0, .5, .5);

	m_herostats_tile.SetWindowSectionPtr(this);
	m_herostats_tile.SetTileType(TileType::FRAME);
	m_herostats_tile.SetRelativeLocation(.5, 0, .5, .5);

	m_fight_ended = false;
	m_enable = false;
	m_sub_mode = FightPickMode::PICK_NOMODE;
	m_hero_turn = true;
	m_hero_turn_idx = 0;
}

bool FightMode::SetTextHandler(Text *texthandler)
{
	m_texthandler_ptr = texthandler;
	return true;
}

bool FightMode::SetPrimaryShader(GLuint primary_shader)
{
	m_primary_shaderid = primary_shader;
	return true;
}

void FightMode::Refresh()
{
	if (!m_enable) return;

	DrawTopWindow();
	DrawMobWindow();
	DrawHeroWindow();

	WindowSection::Refresh();
}

bool FightMode::HandleKey(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_E)
		{
			m_fight_ended = true;
		}
		else if (key == GLFW_KEY_DOWN)
		{
			if (m_sub_mode == FightPickMode::PICK_ACTION)
			{
				m_cur_action_idx++;
				std::vector<std::string> fo = m_battle.GetFightOptions();
				if (m_cur_action_idx >= fo.size())
				{
					m_cur_action_idx = static_cast<int>(fo.size()) - 1;
				}
			}
			else if (m_sub_mode == FightPickMode::PICK_MOB)
			{
				int start_idx = m_cur_mob_idx + 1;
				bool found = m_battle.LookForAliveMob(start_idx, 1, m_cur_mob_idx);
			}
			else if (m_sub_mode == FightPickMode::PICK_ITEM)
			{
				m_cur_item_idx++;
				GameMap *mapptr = GameMap::GetInstance();
				std::vector<Character *> heroes;
				mapptr->GetCurHeroes(heroes);
				if (m_cur_item_idx >= heroes[m_hero_turn_idx]->GetNItems())
				{
					m_cur_item_idx = static_cast<int>(heroes[m_hero_turn_idx]->GetNItems()) - 1;
				}
			}
			else if (m_sub_mode == FightPickMode::PICK_ITEM_TARGET)
			{
				GameMap *mapptr = GameMap::GetInstance();
				std::vector<Character *> heroes;
				mapptr->GetCurHeroes(heroes);
				const Item &curitem = heroes[m_hero_turn_idx]->GetItemRef(m_cur_item_idx);
				if (curitem.GetType() == UseType::FRIENDLY_SINGLE)
				{
					m_cur_item_target_idx++;
					if (m_cur_item_target_idx >= heroes.size())
					{
						m_cur_item_target_idx = static_cast<int>(heroes.size() - 1);
					}
				}
				else if (curitem.GetType() == UseType::VS_SINGLE)
				{
					int start_idx = m_cur_item_target_idx + 1;
					bool found = m_battle.LookForAliveMob(start_idx, 1, m_cur_item_target_idx);
				}
			}
		}
		else if (key == GLFW_KEY_UP)
		{
			if (m_sub_mode == FightPickMode::PICK_ACTION)
			{
				m_cur_action_idx--;
				if (m_cur_action_idx < 0) m_cur_action_idx = 0;
			}
			else if (m_sub_mode == FightPickMode::PICK_MOB)
			{
				int start_idx = m_cur_mob_idx - 1;
				bool found = m_battle.LookForAliveMob(start_idx, -1, m_cur_mob_idx);
			}
			else if (m_sub_mode == FightPickMode::PICK_ITEM)
			{
				m_cur_item_idx--;
				if (m_cur_item_idx < 0)	m_cur_item_idx = 0;
			}
			else if (m_sub_mode == FightPickMode::PICK_ITEM_TARGET)
			{
				int start_idx = m_cur_item_target_idx - 1;
				bool found = m_battle.LookForAliveMob(start_idx, -1, m_cur_item_target_idx);
			}
		}
		else if (key == GLFW_KEY_ENTER)
		{
			if (m_sub_mode == FightPickMode::PICK_ACTION)
			{
				if (m_cur_action_idx == 0)
				{
					//single mob fight
					m_sub_mode = FightPickMode::PICK_MOB;
				}
				else if (m_cur_action_idx == 1)
				{
					//special attack (probably single)
					m_sub_mode = FightPickMode::PICK_MOB;
				}
				else if (m_cur_action_idx == 2)
				{
					//item select
					m_sub_mode = FightPickMode::PICK_ITEM;
				}
				else
				{
					BattleEvent be_retreat;
					be_retreat.SetAsRetreatEvent(m_hero_turn_idx);
					m_battle.AddBattleEvent(be_retreat);
					AdvanceToNextHero();
				}
			}
			else if (m_sub_mode == FightPickMode::PICK_MOB)
			{
				ActionType tmp = static_cast<ActionType>(m_cur_action_idx);
				BattleEvent be_single_mob(true, false, false, m_hero_turn_idx, m_cur_mob_idx, tmp, m_cur_item_idx);
				m_battle.AddBattleEvent(be_single_mob);
				AdvanceToNextHero();
			}
			else if (m_sub_mode == FightPickMode::PICK_ITEM)
			{
				GameMap *mapptr = GameMap::GetInstance();
				std::vector<Character *> heroes;
				mapptr->GetCurHeroes(heroes);
				const Item &curitem = heroes[m_hero_turn_idx]->GetItemRef(m_cur_item_idx);
				if (curitem.GetType() == UseType::FRIENDLY_SINGLE || curitem.GetType() == UseType::VS_SINGLE)
				{
					m_cur_item_target_idx = 0;
					m_sub_mode = FightPickMode::PICK_ITEM_TARGET;
				}
				else if (curitem.GetType() == UseType::FRIENDLY_AOE || curitem.GetType() == UseType::VS_AOE)
				{
					bool is_aoe_target_heroes = (curitem.GetType() == UseType::FRIENDLY_AOE);
					ActionType tmp = static_cast<ActionType>(m_cur_action_idx);
					BattleEvent aoe_event(true, true, is_aoe_target_heroes, m_hero_turn_idx, -1, tmp, m_cur_item_idx);
					m_battle.AddBattleEvent(aoe_event);
					AdvanceToNextHero();
				}
			}
			else if (m_sub_mode == FightPickMode::PICK_ITEM_TARGET)
			{
				GameMap *mapptr = GameMap::GetInstance();
				std::vector<Character *> heroes;
				mapptr->GetCurHeroes(heroes);
				const Item &curitem = heroes[m_hero_turn_idx]->GetItemRef(m_cur_item_idx);
				if (curitem.GetType() == UseType::FRIENDLY_SINGLE || curitem.GetType() == UseType::VS_SINGLE)
				{
					bool is_target_a_hero = (curitem.GetType() == UseType::FRIENDLY_SINGLE);
					ActionType tmp = static_cast<ActionType>(m_cur_action_idx);
					BattleEvent item_use_event(true, false, false, m_hero_turn_idx, m_cur_item_target_idx, tmp, m_cur_item_idx);
					m_battle.AddBattleEvent(item_use_event);
					AdvanceToNextHero();
				}
			}
			if (!m_hero_turn)
			{
				m_battle.AddMobOffenseEvents();
				BattleRoundOutcome battle_round_outcome;
				m_battle.ProcessQueue(battle_round_outcome);
				OutcomeType round_outcome = battle_round_outcome.GetOutcomeType();
				if (round_outcome == OutcomeType::BATTLE_ONGOING)
				{
					m_hero_turn_idx = -1;
					StartBattleRound();
				}
				else if (round_outcome == OutcomeType::ALL_HEROES_DIED)
				{
					std::cout << "Boooo! All Heroes Died!" << std::endl;
					m_fight_ended = true;
				}
				else if (round_outcome == OutcomeType::ALL_MOBS_DIED)
				{
					std::cout << "Yay! All Mobs Died!" << std::endl;
					std::cout << "Loot:" << std::endl;
					std::cout << battle_round_outcome.GetLoot().GetLootString();
					m_fight_ended = true;
				}
				else if (round_outcome == OutcomeType::HEROES_RETREATED)
				{
					std::cout << "Heroes Ran Away Bravely!" << std::endl;
					m_fight_ended = true;

				}
				else if (round_outcome == OutcomeType::MOBS_RETREATED)
				{
					std::cout << "Mobs Ran Away Cowardly!" << std::endl;
					m_fight_ended = true;
				}
			}
		}
	}
	return true;
}

void FightMode::StartFight()
{
	GameMap *gm = GameMap::GetInstance();
	Character *mainchar = gm->GetMainCharPtr();

	m_fight_ended = false;
	m_battle.ClearFightEnded();
	std::vector<Character *> tmp;
	gm->GetCurHeroes(tmp);

	m_battle.Clear();
	m_battle.SetHeroes(tmp);

	int dummy;
	bool is_party_alive = m_battle.LookForActiveHero(0, 1, dummy);
	if (is_party_alive)
	{
		m_battle.SetMobs(gm->GetMobs(mainchar->GetX(), mainchar->GetY()));
		StartBattleRound();
	}
	else
	{
		m_fight_ended = true;
	}

}

void FightMode::StartBattleRound()
{
	m_hero_turn_idx = -1;
	AdvanceToNextHero();

	m_cur_action_idx = 0;
	m_battle.LookForAliveMob(0, 1, m_cur_mob_idx);
	m_cur_item_idx = 0;
	m_cur_item_target_idx = 0;
	m_fight_ended = false;
	m_hero_anim_pct = 0;
	m_hero_anim_dir = -1;

	bool m_hero_turn = true;
	m_sub_mode = FightPickMode::PICK_ACTION;
}

void FightMode::AdvanceToNextHero()
{
	do
	{
		m_hero_turn_idx++;
		if (m_hero_turn_idx >= m_battle.GetNHeroes())
		{
			m_hero_turn = false;
			m_hero_turn_idx = 0;
			m_cur_action_idx = 0;
			m_cur_item_idx = 0;
			m_cur_item_target_idx = 0;
			m_cur_mob_idx = 0;
			m_sub_mode = FightPickMode::PICK_NOMODE;
			return;
		}

		if (m_battle.IsHeroActive(m_hero_turn_idx))
		{
			m_hero_turn = true;
			m_cur_action_idx = 0;
			m_cur_item_idx = 0;
			m_cur_item_target_idx = 0;
			m_battle.LookForAliveMob(0, 1, m_cur_mob_idx);
			m_hero_anim_pct = 0;
			m_hero_anim_dir = -1;
			m_sub_mode = FightPickMode::PICK_ACTION;
			return;
		}

	} while (m_hero_turn_idx <= m_battle.GetNHeroes() && m_hero_turn == true);
}
	
bool FightMode::FightEnded()
{ 
	return m_fight_ended; 
}


void FightMode::DrawTopWindow()
{
	glUseProgram(m_primary_shaderid);

	int n_instances = 1;
	glm::mat4 *translations = new glm::mat4[n_instances];

	double ox = m_top_tile.GetRelativeOriginX_01();
	double oy = m_top_tile.GetRelativeOriginY_01();
	double xd = m_top_tile.GetRelativeWidth_01();
	double yd = m_top_tile.GetRelativeHeight_01();
	double mx = ox + (xd / 2.0);
	double my = oy + (yd / 2.0);
	double mx2 = mx - .5;
	double my2 = my - .5;
	glm::vec3 scalevec(m_top_tile.GetRelativeWidth_01() * 2,
		               m_top_tile.GetRelativeHeight_01() * 2, 1);
	mx2 *= scalevec.x;
	my2 *= scalevec.y;
	glm::vec3 transvec(2 * mx2, 2 * my2, 0);
	translations[0] = glm::translate(glm::mat4(1.0), transvec);
	m_top_tile.SetUpInstancing(1, scalevec, translations);
	m_top_tile.Render();
	delete[] translations;

	GameMap *gm_ptr = GameMap::GetInstance();
	std::vector<Character *> cur_heroes;
	gm_ptr->GetCurHeroes(cur_heroes);

	glm::vec3 char_scalevec(m_top_tile.GetRelativeWidth_01() * 2,
		                    m_top_tile.GetRelativeHeight_01() * 2, 1);
	char_scalevec.x *= .1f;
	char_scalevec.y *= .2f;

	for (int cc = 0; cc < cur_heroes.size(); cc++)
	{
		float xdrawpos = static_cast<float>(m_top_tile.GetXDrawPos_N11());
		float ydrawpos = static_cast<float>(m_top_tile.GetYDrawPos_N11());
		float charoriginx = xdrawpos + (static_cast<float>(m_top_tile.GetRelativeWidth_01()) * 2.0f) * .85f;
		float charoriginy = ydrawpos + (static_cast<float>(m_top_tile.GetRelativeHeight_01()) * 2.0f) * (.8f - (cc * .22f));
		Mesh curchar;
		curchar.LoadMesh(GetMeshFilename(cur_heroes[cc]->GetTileType()));

		if (!cur_heroes[cc]->IsDead())
		{
			curchar.UseTexture(GetTextureFilename(cur_heroes[cc]->GetTileType()));
		}
		else
		{
			curchar.UseTexture(GetTextureFilename(TileType::MUD));
		}
			
		glm::mat4 *translations = new glm::mat4[1];

		if (m_hero_turn && m_hero_turn_idx == cc)
		{
			charoriginx = charoriginx + (static_cast<float>(m_top_tile.GetRelativeWidth_01()) * 
				                        m_hero_anim_dir * (.22f * m_hero_anim_pct / 100.0f));
			m_hero_anim_pct += 10;
			if (m_hero_anim_pct > 100)
			{
				m_hero_anim_pct = 100;
			}
		}

		translations[0] =
			glm::translate(glm::mat4(1.0),
			glm::vec3(charoriginx, charoriginy, 0));
		curchar.SetUpInstancing(1, char_scalevec, translations);
		delete[] translations;
		curchar.Render();

		if (m_sub_mode == FightPickMode::PICK_ITEM_TARGET)
		{
			const Item &curitem = cur_heroes[m_hero_turn_idx]->GetItemRef(m_cur_item_idx);
			if (curitem.GetType() == UseType::FRIENDLY_SINGLE)
			{
				if (m_cur_item_target_idx == cc)
				{
					float arrowxpos = static_cast<float>(m_top_tile.GetXDrawPos_N11()) + 
						                          static_cast<float>(m_top_tile.GetRelativeWidth_01() * 2.0f) * .67f;
					float arrowypos = charoriginy;
					m_texthandler_ptr->Render("-->", arrowxpos, arrowypos, TextAlignType::LEFT);
					glUseProgram(m_primary_shaderid);
				}
			}
			else if (curitem.GetType() == UseType::VS_SINGLE)
			{
				if (m_cur_item_target_idx == cc)
				{
					float arrowxpos = static_cast<float>(m_top_tile.GetXDrawPos_N11()) +
						                         (static_cast<float>(m_top_tile.GetRelativeWidth_01()) * 2.0f) * .3f;
					float arrowypos = charoriginy;
					m_texthandler_ptr->Render("<--", arrowxpos, arrowypos, TextAlignType::LEFT);
					glUseProgram(m_primary_shaderid);
				}
			}
		}
	}

	for (int dd = 0; dd < m_battle.GetNMobs(); dd++)
	{
		float xdrawpos = static_cast<float>(m_top_tile.GetXDrawPos_N11());
		float ydrawpos = static_cast<float>(m_top_tile.GetYDrawPos_N11());
		float moboriginx = xdrawpos + (static_cast<float>(m_top_tile.GetRelativeWidth_01()) * 2.0f) * .15f;
		float moboriginy = ydrawpos + (static_cast<float>(m_top_tile.GetRelativeHeight_01()) * 2.0f) * (.8f - (dd * .22f));
		Mesh curmob;
		curmob.LoadMesh(GetMeshFilename(m_battle.GetMobTileType(dd)));

		if (!m_battle.IsMobDead(dd))
		{
			curmob.UseTexture(GetTextureFilename(m_battle.GetMobTileType(dd)));
		}
		else
		{
			curmob.UseTexture(GetTextureFilename(TileType::MUD));
		}

		glm::mat4 *translations = new glm::mat4[1];

		translations[0] =
			glm::translate(glm::mat4(1.0),
			glm::vec3(moboriginx, moboriginy, 0));
		curmob.SetUpInstancing(1, char_scalevec, translations);
		delete[] translations;
		curmob.Render();

		if (m_sub_mode == FightPickMode::PICK_MOB)
		{
			if (m_cur_mob_idx == dd)
			{
				float arrowxpos = static_cast<float>(m_top_tile.GetXDrawPos_N11()) + 
					                         static_cast<float>((m_top_tile.GetRelativeWidth_01()) * 2.0f) * .3f;
				float arrowypos = moboriginy;
				m_texthandler_ptr->Render("<--", arrowxpos, arrowypos, TextAlignType::LEFT);
				glUseProgram(m_primary_shaderid);
			}
		}

	}
}

void FightMode::DrawMobWindow()
{
	glUseProgram(m_primary_shaderid);

	int n_instances = 1;
	glm::mat4 *translations = new glm::mat4[n_instances];

	double ox = m_mobstats_tile.GetRelativeOriginX_01();
	double oy = m_mobstats_tile.GetRelativeOriginY_01();
	double xd = m_mobstats_tile.GetRelativeWidth_01();
	double yd = m_mobstats_tile.GetRelativeHeight_01();
	double mx = ox + (xd / 2.0);
	double my = oy + (yd / 2.0);
	double mx2 = mx - .5;
	double my2 = my - .5;
	glm::vec3 scalevec(m_mobstats_tile.GetRelativeWidth_01() * 2, 
		               m_mobstats_tile.GetRelativeHeight_01() * 2, 1);
	mx2 *= scalevec.x;
	my2 *= scalevec.y;
	glm::vec3 transvec(2 * mx2, 2 * my2, 0);
	translations[0] = glm::translate(glm::mat4(1.0), transvec);
	m_mobstats_tile.SetUpInstancing(1, scalevec, translations);
	m_mobstats_tile.Render();
	delete[] translations;

	if (m_texthandler_ptr != nullptr)
	{
		float xdrawpos = static_cast<float>(m_mobstats_tile.GetXDrawPos_N11());
		float ydrawpos = static_cast<float>(m_mobstats_tile.GetYDrawPos_N11());
		float textxpos = xdrawpos + (static_cast<float>(m_mobstats_tile.GetRelativeWidth_01()) * 2.0f) * .08f;

		for (int curmon = 0; curmon < m_battle.GetNMobs(); curmon++)
		{
 			float textypos = ydrawpos +
				                     static_cast<float>((m_mobstats_tile.GetRelativeHeight_01()) * 2.0f) * (.85f - (MOB_VERT_SPACING * curmon));
			std::string text = m_battle.GetMobName(curmon);
			if (m_battle.IsMobDead(curmon))
			{
				text += " (Dead)";
			}
			else
			{
				text += " (" + m_battle.GetMobPtr(curmon)->GetHPString() + ")";
			}
			m_texthandler_ptr->Render(text, textxpos, textypos, TextAlignType::LEFT);
		}
	}
	
	if (m_sub_mode == FightPickMode::PICK_ACTION || m_sub_mode == FightPickMode::PICK_ITEM && m_texthandler_ptr != nullptr)
	{
		std::vector<std::string> options;
		int cur_idx(0);
		if (m_sub_mode == FightPickMode::PICK_ACTION)
		{
			options = m_battle.GetFightOptions();
			cur_idx = m_cur_action_idx;
		}
		else if (m_sub_mode == FightPickMode::PICK_ITEM)
		{
			cur_idx = m_cur_item_idx;
			GameMap *mapptr = GameMap::GetInstance();
			std::vector<Character *> heroes;
			mapptr->GetCurHeroes(heroes);
			size_t nitems = heroes[m_hero_turn_idx]->GetNItems();
			for (int aa = 0; aa < nitems; aa++)
			{
				const Item &curitem = heroes[m_hero_turn_idx]->GetItemRef(aa);
				options.emplace_back(curitem.GetName());
			}
		}
			
		float xdrawpos = static_cast<float>(m_mobstats_tile.GetXDrawPos_N11()) +
			                        static_cast<float>((m_mobstats_tile.GetRelativeWidth_01()) * 2.0f) * .6f;
		float ydrawpos = static_cast<float>(m_mobstats_tile.GetYDrawPos_N11());

		for (int aa = 0; aa < options.size(); aa++)
		{
		    float textypos = ydrawpos + (static_cast<float>(m_mobstats_tile.GetRelativeHeight_01()) * 2.0f) *
			                         	   (.85f - (MOB_VERT_SPACING * aa));
			if (cur_idx == aa)
			{
				float arrowxpos = static_cast<float>(m_mobstats_tile.GetXDrawPos_N11()) + 
					                          static_cast<float>((m_mobstats_tile.GetRelativeWidth_01()) * 2.0f) * .55f;
				m_texthandler_ptr->Render("-->", arrowxpos, textypos, TextAlignType::LEFT);
			}
			m_texthandler_ptr->Render(options[aa], xdrawpos, textypos, TextAlignType::LEFT);
		}
	}
}

void FightMode::DrawHeroWindow()
{
	glUseProgram(m_primary_shaderid);

	int n_instances = 1;
	glm::mat4 *translations = new glm::mat4[n_instances];

	double ox = m_herostats_tile.GetRelativeOriginX_01();
	double oy = m_herostats_tile.GetRelativeOriginY_01();
	double xd = m_herostats_tile.GetRelativeWidth_01();
	double yd = m_herostats_tile.GetRelativeHeight_01();
	double mx = ox + (xd / 2.0);
	double my = oy + (yd / 2.0);
	double mx2 = mx - .5;
	double my2 = my - .5;
	glm::vec3 scalevec(m_herostats_tile.GetRelativeWidth_01() * 2, m_herostats_tile.GetRelativeHeight_01() * 2, 1);
	mx2 *= scalevec.x;
	my2 *= scalevec.y;
	glm::vec3 transvec(2 * mx2, 2 * my2, 0);
	translations[0] = glm::translate(glm::mat4(1.0), transvec);
	m_herostats_tile.SetUpInstancing(1, scalevec, translations);
	m_herostats_tile.Render();
	delete[] translations;

	GameMap *gm_ptr = GameMap::GetInstance();
	std::vector<Character *> cur_heroes;
	gm_ptr->GetCurHeroes(cur_heroes);

	if (m_texthandler_ptr != nullptr)
	{
		float xdrawpos = static_cast<float>(m_herostats_tile.GetXDrawPos_N11());
		float ydrawpos = static_cast<float>(m_herostats_tile.GetYDrawPos_N11());
		float textxpos = xdrawpos + (static_cast<float>(m_herostats_tile.GetRelativeWidth_01()) * 2.0f) * .08f;

		for (int cc = 0; cc < cur_heroes.size(); cc++)
		{
			float textypos = ydrawpos + (static_cast<float>(m_herostats_tile.GetRelativeHeight_01()) * 2.0f) * 
				              (.85f - (HERO_VERT_SPACING * cc));
			m_texthandler_ptr->Render(cur_heroes[cc]->GetName(), textxpos, textypos, TextAlignType::LEFT);
			std::string textstring = "(" + cur_heroes[cc]->GetHPString() + ")";
			m_texthandler_ptr->Render(textstring, .25, textypos, TextAlignType::LEFT);
		}
	}
}