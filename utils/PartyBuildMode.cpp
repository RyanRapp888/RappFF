#include "PartyBuildMode.h"
#include <glad.h>
#include <GLFW/glfw3.h>
#include <sstream>

PartyBuildMode::PartyBuildMode(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct) :
WindowSection(vpt, origxpct, origypct, w_pct, h_pct), m_texthandler_ptr(nullptr), m_primary_shaderid(0)
{
	m_drawparty_tile.SetWindowSectionPtr(this);
	m_drawparty_tile.SetTileType(TileType::GRASS);
	m_drawparty_tile.SetRelativeLocation(0, 0, 1, 1);
	m_enable = false;
	m_build_complete = false;
	m_cur_hero_idx = 0;
	m_heros_to_build = 2;
	m_select_type == PartyBuildSelectType::CHARACTER_CHOOSE;
	m_charnames.resize(m_heros_to_build);
}

bool PartyBuildMode::SetTextHandler(Text *texthandler)
{
	m_texthandler_ptr = texthandler;
	return true;
}

bool PartyBuildMode::SetPrimaryShader(GLuint primary_shader)
{
	m_primary_shaderid = primary_shader;
	return true;
}

void PartyBuildMode::Refresh()
{
	if (!m_enable) return;

	DrawPartyBuildWindow();
	
	WindowSection::Refresh();
}

bool PartyBuildMode::IsPartyBuildComplete() const
{
	return m_build_complete;
}

bool PartyBuildMode::HandleKey(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (m_select_type == PartyBuildSelectType::TYPE_NAME &&
			((key >= GLFW_KEY_0 && key <= GLFW_KEY_9) ||
			(key >= GLFW_KEY_A && key <= GLFW_KEY_Z)))
		{
			m_charnames[m_cur_hero_idx] += static_cast<char>(key);
		}
		else if (m_select_type == PartyBuildSelectType::TYPE_NAME &&
			key == GLFW_KEY_BACKSLASH)
		{
			m_charnames[m_cur_hero_idx].erase(m_charnames[m_cur_hero_idx].back());
		}
		else if (key == GLFW_KEY_DOWN)
		{
			if (m_select_type == PartyBuildSelectType::CHARACTER_CHOOSE)
			{
				m_cur_hero_idx++;
				if (m_cur_hero_idx >= m_heros_to_build) m_cur_hero_idx = m_heros_to_build - 1;
			}
		}
		else if (key == GLFW_KEY_UP)
		{
			if (m_select_type == PartyBuildSelectType::CHARACTER_CHOOSE)
			{
				m_cur_hero_idx--;
				if (m_cur_hero_idx < 0) m_cur_hero_idx = 0;
			}
		}
		else if (key == GLFW_KEY_ENTER)
		{
			if (m_select_type == PartyBuildSelectType::CHARACTER_CHOOSE)
			{
				m_select_type = PartyBuildSelectType::TYPE_NAME;
			}
			else if (m_select_type == PartyBuildSelectType::TYPE_NAME)
			{
				m_select_type = PartyBuildSelectType::CHARACTER_CHOOSE;
				m_cur_hero_idx++;
				if (m_cur_hero_idx >= m_heros_to_build) m_cur_hero_idx++;
			}
		}
	}
	return true;
}

void PartyBuildMode::DrawPartyBuildWindow()
{
	glUseProgram(m_primary_shaderid);

	int n_instances = 1;
	glm::mat4 *translations = new glm::mat4[n_instances];

	double ox = m_drawparty_tile.GetRelativeOriginX_01();
	double oy = m_drawparty_tile.GetRelativeOriginY_01();
	double xd = m_drawparty_tile.GetRelativeWidth_01();
	double yd = m_drawparty_tile.GetRelativeHeight_01();
	double mx = ox + (xd / 2.0);
	double my = oy + (yd / 2.0);
	double mx2 = mx - .5;
	double my2 = my - .5;

	glm::vec3 scalevec(m_drawparty_tile.GetRelativeWidth_01() * 2,
		m_drawparty_tile.GetRelativeHeight_01() * 2, 1);
	mx2 *= scalevec.x;
	my2 *= scalevec.y;
	glm::vec3 transvec(2 * mx2, 2 * my2, 0);
	translations[0] = glm::translate(glm::mat4(1.0), transvec);
	m_drawparty_tile.SetUpInstancing(1, scalevec, translations);
	m_drawparty_tile.Render();
	delete[] translations;
	/*
	std::vector<Character *> heros_elect;
	
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

		if (m_sub_mode == FightPickMode::PICK_ITEM_TARGET)
		{
			const Item &curitem = cur_heroes[m_hero_turn_idx]->GetItemRef(m_cur_item_idx);
			if (curitem.GetType() == UseType::FRIENDLY_SINGLE)
			{
				if (m_cur_item_target_idx == cc)
				{
					double arrowxpos = m_top_tile.GetXDrawPos_N11() + (m_top_tile.GetRelativeWidth_01() * 2) * .67;
					double arrowypos = charoriginy;
					m_texthandler_ptr->Render("-->", arrowxpos, arrowypos, TextAlignType::LEFT);
					glUseProgram(m_primary_shaderid);
				}
			}
			else if (curitem.GetType() == UseType::VS_SINGLE)
			{
				if (m_cur_item_target_idx == cc)
				{
					double arrowxpos = m_top_tile.GetXDrawPos_N11() + (m_top_tile.GetRelativeWidth_01() * 2) * .3;
					double arrowypos = charoriginy;
					m_texthandler_ptr->Render("<--", arrowxpos, arrowypos, TextAlignType::LEFT);
					glUseProgram(m_primary_shaderid);
				}
			}
		}
	}

	for (int dd = 0; dd < m_battle.GetNMobs(); dd++)
	{
		double xdrawpos = m_top_tile.GetXDrawPos_N11();
		double ydrawpos = m_top_tile.GetYDrawPos_N11();
		double moboriginx = xdrawpos + (m_top_tile.GetRelativeWidth_01() * 2) * .15;
		double moboriginy = ydrawpos + (m_top_tile.GetRelativeHeight_01() * 2) * (.8 - (dd * .22));
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
				double arrowxpos = m_top_tile.GetXDrawPos_N11() + (m_top_tile.GetRelativeWidth_01() * 2) * .3;
				double arrowypos = moboriginy;
				m_texthandler_ptr->Render("<--", arrowxpos, arrowypos, TextAlignType::LEFT);
				glUseProgram(m_primary_shaderid);
			}
		}

	}
	*/
}
