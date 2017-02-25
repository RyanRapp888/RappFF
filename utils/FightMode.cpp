#include "FightMode.h"
#include <glad.h>
#include <GLFW/glfw3.h>
#include <sstream>

FightMode::FightMode(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct) :
WindowSection(vpt, origxpct, origypct, w_pct, h_pct), m_texthandler_ptr(nullptr), m_primary_shaderid(0)
{
	Tile *bltile = &(m_five_window_parts[0]);
	bltile->SetTileType(TileType::FRAME);
	bltile->SetWindowSectionPtr(this);
	bltile->SetRelativeLocation(0, 0, .5, .5);

	Tile *brtile = &(m_five_window_parts[1]);
	brtile->SetWindowSectionPtr(this);
	brtile->SetTileType(TileType::FRAME);
	brtile->SetRelativeLocation(.5, 0, .5, .5);

	Tile *top = &(m_five_window_parts[2]);
	top->SetWindowSectionPtr(this);
	top->SetTileType(TileType::FRAME);
	top->SetRelativeLocation(0, .5, 1, .5);

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

void FightMode::Refresh()
{
	if (!m_enable) return;
		
	for (int aa = 0; aa < N_FRAMES; aa++)
	{
		Tile *cur = &(m_five_window_parts[aa]);
				
		if (m_texthandler_ptr != nullptr)
		{
			double xdrawpos = cur->GetXDrawPos_N11();
			// get the origin point -1 to 1
			double ydrawpos = cur->GetYDrawPos_N11();
			double winxmid = xdrawpos + (cur->GetRelativeWidth_01() * 2) / 2.0;
			double winymid = ydrawpos + (cur->GetRelativeHeight_01() * 2) / 2.0;
			std::ostringstream util;
			util << aa;
							
			m_texthandler_ptr->Render("Window #" + util.str(), winxmid, 
									  winymid, TextAlignType::LEFT);
		}
		
		glUseProgram(m_primary_shaderid);
		
		int n_instances = 1;
		glm::mat4 *translations = new glm::mat4[n_instances];
				
		double ox = cur->GetRelativeOriginX_01();
		double oy = cur->GetRelativeOriginY_01();
		double xd = cur->GetRelativeWidth_01();
		double yd = cur->GetRelativeHeight_01();
		double mx = ox + (xd / 2.0);
		double my = oy + (yd / 2.0);
		double mx2 = mx - .5;
		double my2 = my - .5;
		glm::vec3 scalevec(cur->GetRelativeWidth_01() * 2, cur->GetRelativeHeight_01() * 2, 1);
		mx2 *= scalevec.x;
		my2 *= scalevec.y;
		glm::vec3 transvec(2 * mx2,2 * my2, 0);
		translations[0] = glm::translate(glm::mat4(1.0), transvec);
		cur->SetUpInstancing(1, scalevec, translations);
		cur->Render();
		delete[] translations;
	}

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