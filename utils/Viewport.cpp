#include "Viewport.h"
#include "WindowSection.h"
#include <glad.h>
#include "GLFW\glfw3.h"

Viewport *WindowSection::GetVP()
{
	return m_view_ptr;
}

void Viewport::SetWindowSize(int w, int h)
{
	m_win_width = w;
	m_win_height = h;
	glViewport(0, 0, w, h);
}

bool Viewport::Refresh()
{
	for (auto vpt_owned_obj : m_drawables)
	{
		vpt_owned_obj->Render();
	}

	for (auto cur_win_section : m_win_sections)
	{
		cur_win_section->Refresh();
	}

	return true;
}

void Viewport::AddWindowSection(WindowSection *dat)
{
	dat->m_view_ptr = this;
	m_win_sections.push_back(dat);
}

void Viewport::RemoveWindowSection(WindowSection *dat)
{
	auto iter = std::find(m_win_sections.begin(), m_win_sections.end(), dat);
	if (iter != m_win_sections.end())
	{
		(*iter)->m_view_ptr = nullptr;
		m_win_sections.erase(iter);
	}
}

void Viewport::AddObject(DrawableObj *dat)
{
	dat->m_viewport_ptr = this;
	dat->m_winsection_ptr = nullptr;
	m_drawables.push_back(dat);
}

void Viewport::RemoveObject(DrawableObj *dat)
{
	auto iter = std::find(m_drawables.begin(), m_drawables.end(), dat);
	if (iter != m_drawables.end())
	{
		//iter->m_viewport_ptr = nullptr;
		//iter->m_winsection_ptr = nullptr;
		m_drawables.erase(iter);
	}
}

int Viewport::GetWinWidth()
{
	return m_win_width;
}

int Viewport::GetWinHeight()
{
	return m_win_height;
}

WindowSection *Viewport::FindWindowSection(int win_x, int win_y)
{
	int act_y = m_win_height - win_y;
	int act_x = win_x;

	double x_pct = act_x / (double)m_win_width;
	double y_pct = act_y / (double)m_win_height;

	for (int aa = 0; aa < m_win_sections.size(); aa++)
	{
		WindowSection *cur = m_win_sections[aa];
		if (x_pct >= cur->GetOriginX_01() &&
			x_pct <= (cur->GetOriginX_01() + cur->GetWidth_01()))
		{
			if (y_pct >= cur->GetOriginY_01() &&
				y_pct <= (cur->GetOriginY_01() + cur->GetHeight_01()))
			{
				return cur;
			}
		}
	}
	return nullptr;
}
