#include "DrawableObj.h"

DrawableObj &DrawableObj::operator=(const DrawableObj & other)
{
	if (this != &other) // protect against invalid self-assignment
	{
		this->m_rel_originx_01 = other.m_rel_originx_01;
		this->m_rel_originy_01 = other.m_rel_originy_01;
		this->m_rel_width_01 = other.m_rel_width_01;
		this->m_rel_height_01 = other.m_rel_height_01;
		if (other.m_winsection_ptr != nullptr) this->m_winsection_ptr = other.m_winsection_ptr;
		if (other.m_viewport_ptr != nullptr) this->m_viewport_ptr = other.m_viewport_ptr;
	}
	return *this;
}

void DrawableObj::GetRelativeLocation(
	double &originx_01, double &originy_01,
	double &width_01, double &height_01)
{
	originx_01 = m_rel_originx_01;
	originy_01 = m_rel_originy_01;
	width_01 = m_rel_width_01;
	height_01 = m_rel_height_01;
}

void DrawableObj::SetRelativeLocation(
	double originx_01, double originy_01,
	double width_01, double height_01)
{
	m_rel_originx_01 = originx_01;
	m_rel_originy_01 = originy_01;
	m_rel_width_01 = width_01;
	m_rel_height_01 = height_01;
}

void DrawableObj::SetWindowSectionPtr(WindowSection *obj)
{
	m_winsection_ptr = obj;
}

double DrawableObj::GetRelativeWidth_01()
{
	double result(0);
	if (m_winsection_ptr != nullptr)
	{
		result = m_winsection_ptr->GetWidth_01() * m_rel_width_01;
	}
	else
	{
		result = m_rel_width_01;
	}
	return result;
}

double DrawableObj::GetRelativeHeight_01()
{
	double result(0);
	if (m_winsection_ptr != nullptr)
	{
		result = m_winsection_ptr->GetHeight_01() * m_rel_height_01;
	}
	else
	{
		result = m_rel_height_01;
	}
	return result;
}

double DrawableObj::GetRelativeOriginX_01()
{
	double result(m_rel_originx_01);
	if (m_viewport_ptr != nullptr)
	{
		result = m_rel_originx_01;
	}
	else if (m_winsection_ptr != nullptr)
	{
		result = m_winsection_ptr->GetOriginX_01() +
			(m_winsection_ptr->GetWidth_01() * m_rel_originx_01);
	}
	return result;
}

double DrawableObj::GetRelativeOriginY_01()
{
	double result(-77);
	if (m_viewport_ptr != nullptr)
	{
		result = m_rel_originy_01;
	}
	else if (m_winsection_ptr != nullptr)
	{
		result = m_winsection_ptr->GetOriginY_01() +
			(m_winsection_ptr->GetHeight_01() * m_rel_originy_01);
	}
	return result;
}

double DrawableObj::GetXDrawPos_N11()
{
	double result;
	double rr = GetRelativeOriginX_01();
	result = (rr * 2) - 1;
	return result;
}

double DrawableObj::GetYDrawPos_N11()
{
	double result;
	double rr = GetRelativeOriginY_01();
	result = (rr * 2) - 1;
	return result;
}