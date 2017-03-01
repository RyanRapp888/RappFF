#include "FightMode.h"
#include <glad.h>
#include <GLFW/glfw3.h>
#include <sstream>

#define HERO_VERT_SPACING .19
#define MONSTER_VERT_SPACING .1

FightMode::FightMode(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct) :
WindowSection(vpt, origxpct, origypct, w_pct, h_pct), m_texthandler_ptr(nullptr), m_primary_shaderid(0)
{
	m_top_tile.SetWindowSectionPtr(this);
	m_top_tile.SetTileType(TileType::GRASS);
	m_top_tile.SetRelativeLocation(0, .5, 1, .5);

	m_monsterstats_tile.SetTileType(TileType::FRAME);
	m_monsterstats_tile.SetWindowSectionPtr(this);
	m_monsterstats_tile.SetRelativeLocation(0, 0, .5, .5);

	m_herostats_tile.SetWindowSectionPtr(this);
	m_herostats_tile.SetTileType(TileType::FRAME);
	m_herostats_tile.SetRelativeLocation(.5, 0, .5, .5);

	m_enable = false;
	m_fight_ended = false;
	m_sub_mode = FightSubMode::PICK_NOMODE;
	m_cur_action_idx = 0;
	m_cur_item_idx = 0;
	m_cur_item_target_idx = 0;
	m_cur_monster_idx = 0;
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

static std::vector<std::string> GetFightOptions()
{
	std::vector<std::string> results = { "Attack", "Special Move", "Item", "Retreat" };
	return results;
}

void FightMode::StartFight()
{ 
	// Eventually, we can look for "boss fights"
	m_battle_queue.clear();
	m_fight_ended = false;
	GameMap *gm = GameMap::GetInstance();
	m_monsters.clear();
	Character *mainchar = gm->GetMainCharPtr();
	m_monsters = gm->GetMonsters(mainchar->GetX(), mainchar->GetY());
	m_hero_turn = true;
	
	m_hero_turn_idx = -1;
	AdvanceToNextHero();
	
}

void FightMode::Refresh()
{
	if (!m_enable) return;
	
	DrawTopWindow();
	DrawMonsterWindow();
	DrawHeroWindow();

	WindowSection::Refresh();
}

void FightMode::DoFightAction()
{




	return;
}

void FightMode::AdvanceToNextHero()
{
	m_hero_anim_pct = 0;
	
	GameMap *gm_ptr = GameMap::GetInstance();
	std::vector<Character *> cur_heroes;
	gm_ptr->GetCurHeroes(cur_heroes);

	do
	{
		m_hero_turn_idx++;
		if (m_hero_turn_idx >= cur_heroes.size())
		{
			m_hero_turn = false;
			m_hero_turn_idx = 0;
			m_cur_action_idx = 0;
			m_cur_item_idx = 0;
			m_cur_item_target_idx = 0;
			m_cur_monster_idx = 0;
			m_sub_mode = FightSubMode::PICK_NOMODE;
			// Now we are ready to process the queue, and give the monsters a chance.
			break;
		}

		if (cur_heroes[m_hero_turn_idx]->IsActive())
		{
			m_hero_anim_pct = 0;
			m_hero_anim_dir = -1;
			m_cur_action_idx = 0;
			m_cur_item_idx = 0;
			m_cur_item_target_idx = 0;
			m_cur_monster_idx = 0;
			m_sub_mode = FightSubMode::PICK_ACTION;
			break;
		}
		
	} while (m_hero_turn_idx <= cur_heroes.size() && m_hero_turn == true);
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
			if (m_sub_mode == FightSubMode::PICK_ACTION)
			{
				m_cur_action_idx++;
				std::vector<std::string> fo = GetFightOptions();
				if (m_cur_action_idx >= fo.size())
				{
					m_cur_action_idx = fo.size() - 1;
				}
			}
			else if (m_sub_mode == FightSubMode::PICK_MONSTER)
			{
				m_cur_monster_idx++;
				if (m_cur_monster_idx >= m_monsters.size())
				{
					m_cur_monster_idx = m_monsters.size() - 1;
				}
			}
			else if (m_sub_mode == FightSubMode::PICK_ITEM)
			{
				m_cur_item_idx++;
				GameMap *mapptr = GameMap::GetInstance();
				std::vector<Character *> heroes;
				mapptr->GetCurHeroes(heroes);
				if (m_cur_item_idx >= heroes[m_hero_turn_idx]->GetNItems())
				{
					m_cur_item_idx = heroes[m_hero_turn_idx]->GetNItems() - 1;
				}
			}
			else if (m_sub_mode == FightSubMode::PICK_ITEM_TARGET)
			{
				GameMap *mapptr = GameMap::GetInstance();
				std::vector<Character *> heroes;
				mapptr->GetCurHeroes(heroes);
				const Item &curitem = heroes[m_hero_turn_idx]->GetItemRef(m_cur_item_idx);
				if (curitem.GetType() == UseType::FRIENDLY_SINGLE)
				{
					if (m_cur_item_target_idx >= heroes.size())
					{
						m_cur_item_target_idx = heroes.size() - 1;
					}
				}
				else if (curitem.GetType() == UseType::VS_SINGLE)
				{
					if (m_cur_item_target_idx >= m_monsters.size())
					{
						m_cur_item_target_idx = m_monsters.size() - 1;
					}
				}
			}
		}
		else if (key == GLFW_KEY_UP)
		{
			if (m_sub_mode == FightSubMode::PICK_ACTION)
			{
				m_cur_action_idx--;
				if (m_cur_action_idx < 0) m_cur_action_idx = 0;
			}
			else if (m_sub_mode == FightSubMode::PICK_MONSTER)
			{
				m_cur_monster_idx--;
				if (m_cur_monster_idx < 0)	m_cur_monster_idx = 0;
			}
			else if (m_sub_mode == FightSubMode::PICK_ITEM)
			{
				m_cur_item_idx--;
				if (m_cur_item_idx < 0)	m_cur_item_idx = 0;
			}
			else if (m_sub_mode == FightSubMode::PICK_ITEM_TARGET)
			{
				m_cur_item_target_idx--;
				if (m_cur_item_target_idx < 0) m_cur_item_target_idx = 0;
			}
		}
		else if (key == GLFW_KEY_ENTER)
		{
			if (m_sub_mode == FightSubMode::PICK_ACTION)
			{
				if (m_cur_action_idx == 0)
				{
					//single monster fight
					m_sub_mode = FightSubMode::PICK_MONSTER;
				}
				else if (m_cur_action_idx == 1)
				{
					//special attack (probably single)
					m_sub_mode = FightSubMode::PICK_MONSTER;
				}
				else if (m_cur_action_idx == 2)
				{
					//item select
					m_sub_mode = FightSubMode::PICK_ITEM;
				}
				else
				{
					BattleEvent be_retreat;
					be_retreat.SetAsRetreatEvent(m_hero_turn_idx);
					m_battle_queue.emplace_back(be_retreat);
					AdvanceToNextHero();
				}
			}
			else if (m_sub_mode == FightSubMode::PICK_MONSTER)
			{
				BattleEvent be_single_monster(true, false, false, m_hero_turn_idx, m_cur_monster_idx, m_cur_action_idx, m_cur_item_idx);
				m_battle_queue.emplace_back(be_single_monster);
				AdvanceToNextHero();
			}
			else if (m_sub_mode == FightSubMode::PICK_ITEM)
			{
				GameMap *mapptr = GameMap::GetInstance();
				std::vector<Character *> heroes;
				mapptr->GetCurHeroes(heroes);
				const Item &curitem = heroes[m_hero_turn_idx]->GetItemRef(m_cur_item_idx);
				if (curitem.GetType() == UseType::FRIENDLY_SINGLE || curitem.GetType() == UseType::VS_SINGLE)
				{
					m_cur_item_target_idx = 0;
					m_sub_mode = FightSubMode::PICK_ITEM_TARGET;
				}
				else if (curitem.GetType() == UseType::FRIENDLY_AOE || curitem.GetType() == UseType::VS_AOE)
				{
					bool is_aoe_target_heroes = (curitem.GetType() == UseType::FRIENDLY_AOE);
					BattleEvent aoe_event(true, true, is_aoe_target_heroes, m_hero_turn_idx, -1, m_cur_action_idx, m_cur_item_idx);
					m_battle_queue.emplace_back(aoe_event);
					AdvanceToNextHero();
				}
			}
			else if (m_sub_mode == FightSubMode::PICK_ITEM_TARGET)
			{
				GameMap *mapptr = GameMap::GetInstance();
				std::vector<Character *> heroes;
				mapptr->GetCurHeroes(heroes);
				const Item &curitem = heroes[m_hero_turn_idx]->GetItemRef(m_cur_item_idx);
				if (curitem.GetType() == UseType::FRIENDLY_SINGLE || curitem.GetType() == UseType::VS_SINGLE)
				{
					bool is_target_a_hero = (curitem.GetType() == UseType::FRIENDLY_SINGLE);
					BattleEvent item_use_event(true, false, false, m_hero_turn_idx, m_cur_item_target_idx, m_cur_action_idx, m_cur_item_idx);
					m_battle_queue.emplace_back(item_use_event);
					AdvanceToNextHero();
				}
			}
		}
	}
	return true;
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
	char_scalevec.x *= .1;
	char_scalevec.y *= .2;

	for (int cc = 0; cc < cur_heroes.size(); cc++)
	{
		double xdrawpos = m_top_tile.GetXDrawPos_N11();
		double ydrawpos = m_top_tile.GetYDrawPos_N11();
		double charoriginx = xdrawpos + (m_top_tile.GetRelativeWidth_01() * 2) * .85;
		double charoriginy = ydrawpos + (m_top_tile.GetRelativeHeight_01() * 2) * (.8 - (cc * .22));
		Mesh curchar;
		curchar.LoadMesh(GetMeshFilename(cur_heroes[cc]->GetTileType()));
		curchar.UseTexture(GetTextureFilename(cur_heroes[cc]->GetTileType()));
		glm::mat4 *translations = new glm::mat4[1];

		if (m_hero_turn && m_hero_turn_idx == cc)
		{
			charoriginx = charoriginx + (m_top_tile.GetRelativeWidth_01() * 
				                        m_hero_anim_dir * (.22 * m_hero_anim_pct / 100.0));
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
	}
}

void FightMode::DrawMonsterWindow()
{
	glUseProgram(m_primary_shaderid);

	int n_instances = 1;
	glm::mat4 *translations = new glm::mat4[n_instances];

	double ox = m_monsterstats_tile.GetRelativeOriginX_01();
	double oy = m_monsterstats_tile.GetRelativeOriginY_01();
	double xd = m_monsterstats_tile.GetRelativeWidth_01();
	double yd = m_monsterstats_tile.GetRelativeHeight_01();
	double mx = ox + (xd / 2.0);
	double my = oy + (yd / 2.0);
	double mx2 = mx - .5;
	double my2 = my - .5;
	glm::vec3 scalevec(m_monsterstats_tile.GetRelativeWidth_01() * 2, 
		               m_monsterstats_tile.GetRelativeHeight_01() * 2, 1);
	mx2 *= scalevec.x;
	my2 *= scalevec.y;
	glm::vec3 transvec(2 * mx2, 2 * my2, 0);
	translations[0] = glm::translate(glm::mat4(1.0), transvec);
	m_monsterstats_tile.SetUpInstancing(1, scalevec, translations);
	m_monsterstats_tile.Render();
	delete[] translations;

	if (m_texthandler_ptr != nullptr)
	{
		double xdrawpos = m_monsterstats_tile.GetXDrawPos_N11();
		double ydrawpos = m_monsterstats_tile.GetYDrawPos_N11();
		double textxpos = xdrawpos + (m_monsterstats_tile.GetRelativeWidth_01() * 2) * .08;

		for (int curmon = 0; curmon < m_monsters.size(); curmon++)
		{
 			double textypos = ydrawpos + (m_monsterstats_tile.GetRelativeHeight_01() * 2) * (.85 - (MONSTER_VERT_SPACING * curmon));
			m_texthandler_ptr->Render(m_monsters[curmon].GetName(), textxpos, textypos, TextAlignType::LEFT);
		}
	}
	
	if (m_sub_mode == FightSubMode::PICK_ACTION || m_sub_mode == FightSubMode::PICK_ITEM && m_texthandler_ptr != nullptr)
	{
		std::vector<std::string> options;
		int cur_idx(0);
		if (m_sub_mode == FightSubMode::PICK_ACTION)
		{
			options = GetFightOptions();
			cur_idx = m_cur_action_idx;
		}
		else if (m_sub_mode == FightSubMode::PICK_ITEM)
		{
			cur_idx = m_cur_item_idx;
			GameMap *mapptr = GameMap::GetInstance();
			std::vector<Character *> heroes;
			mapptr->GetCurHeroes(heroes);
			int nitems = heroes[m_hero_turn_idx]->GetNItems();
			for (int aa = 0; aa < nitems; aa++)
			{
				const Item &curitem = heroes[m_hero_turn_idx]->GetItemRef(aa);
				options.emplace_back(curitem.GetName());
			}
		}
			
		double xdrawpos = m_monsterstats_tile.GetXDrawPos_N11() + (m_monsterstats_tile.GetRelativeWidth_01() * 2) * .5;
		double ydrawpos = m_monsterstats_tile.GetYDrawPos_N11();

		for (int aa = 0; aa < options.size(); aa++)
		{
		    double textypos = ydrawpos + (m_monsterstats_tile.GetRelativeHeight_01() * 2) *
			                         	   (.85 - (MONSTER_VERT_SPACING * aa));
			if (cur_idx == aa)
			{
				double arrowxpos = m_monsterstats_tile.GetXDrawPos_N11() + (m_monsterstats_tile.GetRelativeWidth_01() * 2) * .45;
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
		double xdrawpos = m_herostats_tile.GetXDrawPos_N11();
		double ydrawpos = m_herostats_tile.GetYDrawPos_N11();
		double textxpos = xdrawpos + (m_herostats_tile.GetRelativeWidth_01() * 2) * .08;

		for (int cc = 0; cc < cur_heroes.size(); cc++)
		{
			double textypos = ydrawpos + (m_herostats_tile.GetRelativeHeight_01() * 2) * 
				              (.85 - (HERO_VERT_SPACING * cc));
			m_texthandler_ptr->Render(cur_heroes[cc]->GetName(), textxpos, textypos, TextAlignType::LEFT);
			m_texthandler_ptr->Render(cur_heroes[cc]->GetHPString(), .35, textypos, TextAlignType::LEFT);
		}
	}
}