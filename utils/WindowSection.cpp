#include "WindowSection.h"
#include <algorithm>

WindowSection::WindowSection(): 
   m_view_ptr(nullptr),
   m_originx_01(0),
   m_originy_01(0),
   m_width_01(0),
   m_height_01(0)
{
}


void WindowSection::Refresh()
{
	for (auto cur_obj : m_drawables)
	{
		cur_obj->Render();
	}
}

void WindowSection::AddObject(DrawableObj *dat)
{
	dat->m_winsection_ptr = this;
	dat->m_viewport_ptr = nullptr;
	m_drawables.push_back(dat);
}

double WindowSection::GetOriginX_01()
{
	return m_originx_01;
}

double WindowSection::GetOriginY_01()
{
	return m_originy_01;
}

double WindowSection::GetWidth_01()
{
	return m_width_01;
}

double WindowSection::GetHeight_01()
{
	return m_height_01;
}

void WindowSection::RemoveObject(DrawableObj *dat)
{
	auto iter = std::find(m_drawables.begin(), m_drawables.end(), dat);
	if (iter != m_drawables.end())
	{
		//iter.m_winsection_ptr = nullptr;
		//iter.m_viewport_ptr = nullptr;
		m_drawables.erase(iter);
	}
}
