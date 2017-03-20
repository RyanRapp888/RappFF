#include "NewOrLoadMode.h"
#include <glad.h>
#include <GLFW/glfw3.h>
#include <sstream>

#define NOL_VERT_SPACING .19

NewOrLoadMode::NewOrLoadMode(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct) :
WindowSection(vpt, origxpct, origypct, w_pct, h_pct), m_texthandler_ptr(nullptr), m_primary_shaderid(0)
{
	m_neworload_tile.SetWindowSectionPtr(this);
	m_neworload_tile.SetTileType(TileType::GRASS);
	m_neworload_tile.SetRelativeLocation(0, 0, 1, 1);
	m_enable = false;
	m_screen_complete = false;
	m_opt_idx = 0;
	m_select_type == NOL_PICK_MODE::NEWORLOAD_PICK;
	m_cur_options.push_back("New Game");
	m_cur_options.push_back("Load Saved Game");
	m_filename = "";
}

bool NewOrLoadMode::SetTextHandler(Text *texthandler)
{
	m_texthandler_ptr = texthandler;
	return true;
}

bool NewOrLoadMode::SetPrimaryShader(GLuint primary_shader)
{
	m_primary_shaderid = primary_shader;
	return true;
}

void NewOrLoadMode::Refresh()
{
	if (!m_enable) return;
	DrawMainScreen();
	WindowSection::Refresh();
}

bool NewOrLoadMode::HandleKey(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_DOWN)
		{
			m_opt_idx++;
			if (m_opt_idx >= m_cur_options.size()) m_opt_idx = m_cur_options.size() - 1;
		}
		else if (key == GLFW_KEY_UP)
		{
			m_opt_idx--;
			if (m_opt_idx < 0) m_opt_idx = 0;
		}
	    else if (key == GLFW_KEY_ENTER)
		{
			if (m_select_type == NOL_PICK_MODE::NEWORLOAD_PICK)
			{
				if (m_opt_idx == 0)
				{
					m_filename = "";
					m_screen_complete = true;
				}
				else
				{
					m_select_type = NOL_PICK_MODE::CHOOSE_LOADABLE;
					m_cur_options.clear();
					m_cur_options.push_back("Save1");
					m_cur_options.push_back("Save2");
					m_cur_options.push_back("Save3");
					m_opt_idx = 0;
				}
			}
			else if (m_select_type == NOL_PICK_MODE::CHOOSE_LOADABLE)
			{
				m_filename = m_cur_options[m_opt_idx];
			}
		}
	}
	return true;
}

void NewOrLoadMode::DrawMainScreen()
{
	glUseProgram(m_primary_shaderid);

	int n_instances = 1;
	glm::mat4 *translations = new glm::mat4[n_instances];

	double ox = m_neworload_tile.GetRelativeOriginX_01();
	double oy = m_neworload_tile.GetRelativeOriginY_01();
	double xd = m_neworload_tile.GetRelativeWidth_01();
	double yd = m_neworload_tile.GetRelativeHeight_01();
	double mx = ox + (xd / 2.0);
	double my = oy + (yd / 2.0);
	double mx2 = mx - .5;
	double my2 = my - .5;
	glm::vec3 scalevec(m_neworload_tile.GetRelativeWidth_01() * 2,
		m_neworload_tile.GetRelativeHeight_01() * 2, 1);
	mx2 *= scalevec.x;
	my2 *= scalevec.y;
	glm::vec3 transvec(2 * mx2, 2 * my2, 0);
	translations[0] = glm::translate(glm::mat4(1.0), transvec);
	m_neworload_tile.SetUpInstancing(1, scalevec, translations);
	m_neworload_tile.Render();
	delete[] translations;

	if (m_texthandler_ptr != nullptr)
	{
		double xdrawpos = m_neworload_tile.GetXDrawPos_N11();
		double ydrawpos = m_neworload_tile.GetYDrawPos_N11();
		double textxpos = xdrawpos + (m_neworload_tile.GetRelativeWidth_01() * 2) * .08;

		for (int dd = 0; dd < m_cur_options.size(); dd++)
		{
			double textypos = ydrawpos + (m_neworload_tile.GetRelativeHeight_01() * 2) * (.85 - (NOL_VERT_SPACING * dd));
			std::string text = m_cur_options[dd];
			if (dd == m_opt_idx)
			{
				int t2 = textxpos + (m_neworload_tile.GetRelativeWidth_01() * 2) * .1;
				m_texthandler_ptr->Render("<==", t2, textypos, TextAlignType::LEFT);
			}
			m_texthandler_ptr->Render(text, textxpos, textypos, TextAlignType::LEFT);
		}
	}

}
