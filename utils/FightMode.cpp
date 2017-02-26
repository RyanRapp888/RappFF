#include "FightMode.h"
#include <glad.h>
#include <GLFW/glfw3.h>
#include <sstream>

#define TEXT_VERT_SPACING .15

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


void FightMode::StartFight()
{ 
	// Eventually, we can look for "boss fights"
	m_fight_ended = false;
	GameMap *gm = GameMap::GetInstance();
	m_monsters.clear();
	Character *mainchar = gm->GetMainCharPtr();
	m_monsters = gm->GetMonsters(mainchar->GetX(), mainchar->GetY());
}

void FightMode::Refresh()
{
	if (!m_enable) return;
		
	DrawTopWindow();
	DrawMonsterStatsWindow();
	DrawHeroStatsWindow();

	WindowSection::Refresh();
}

bool FightMode::HandleKey(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_E)
	{
		m_fight_ended = true;
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
	
	/*
	if (m_texthandler_ptr != nullptr)
	{
		double xdrawpos = m_top_tile.GetXDrawPos_N11();
		double ydrawpos = m_top_tile.GetYDrawPos_N11();
		double textposx = xdrawpos + (m_top_tile.GetRelativeWidth_01() * 2) * .05;
		double textposy = ydrawpos + (m_top_tile.GetRelativeHeight_01() * 2) *.8;
		std::ostringstream util;
		m_texthandler_ptr->Render("Throwdown Zone", textposx, textposy, TextAlignType::LEFT);
	}
	*/
}

void FightMode::DrawMonsterStatsWindow()
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
	glm::vec3 scalevec(m_monsterstats_tile.GetRelativeWidth_01() * 2, m_monsterstats_tile.GetRelativeHeight_01() * 2, 1);
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
 			double textypos = ydrawpos + (m_monsterstats_tile.GetRelativeHeight_01() * 2) * (.8 - (TEXT_VERT_SPACING * curmon));
			m_texthandler_ptr->Render(m_monsters[curmon].GetName(), textxpos, textypos, TextAlignType::LEFT);
		}
	}

}

void FightMode::DrawHeroStatsWindow()
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
			double textypos = ydrawpos + (m_herostats_tile.GetRelativeHeight_01() * 2) * (.8 - (TEXT_VERT_SPACING * cc));
			m_texthandler_ptr->Render(cur_heroes[cc]->GetName(), textxpos, textypos, TextAlignType::LEFT);
		}
	}

}