#include <iostream>
#include <utils/RGB.h>
#include <vector>
#include <algorithm>
#include <utils/WinViewportUtils.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowSection;
class Viewport;

void DrawableObj::SetRelativeLocation(
   double originx_01, double originy_01,
   double width_01,double height_01)
{
   m_rel_originx_01 = originx_01; 
   m_rel_originy_01 = originy_01; 
   m_rel_width_01 = width_01; 
   m_rel_height_01 = height_01;
}
 
void DrawableObj::GetRelativeLocation(
   double &originx_01, double &originy_01,
   double &width_01,double &height_01)
{
   originx_01 = m_rel_originx_01;
   originy_01 = m_rel_originy_01;
   width_01 = m_rel_width_01;
   height_01 = m_rel_height_01;
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
   double result;
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
   double result;
   if (m_viewport_ptr != nullptr)
   {
      result = m_rel_originy_01;
   }
   else if (m_winsection_ptr != nullptr)
   {
      result = m_winsection_ptr->GetOriginY_01() + 
               (m_winsection_ptr->GetHeight_01() * m_rel_originy_01) ;
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

void WindowSection::Refresh()
{
   for(auto cur_obj : m_drawables)
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
   auto iter = std::find(m_drawables.begin(),m_drawables.end(),dat);
   if(iter != m_drawables.end())
   {
     //iter.m_winsection_ptr = nullptr;
     //iter.m_viewport_ptr = nullptr;
     m_drawables.erase(iter);
   }
}
     
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
   for(auto vpt_owned_obj : m_drawables)
   {
      vpt_owned_obj->Render();
   }
   
   for(auto cur_win_section : m_win_sections )
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
   if(iter != m_win_sections.end())
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
   auto iter = std::find(m_drawables.begin(),m_drawables.end(),dat);
   if(iter != m_drawables.end())
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
      
   double x_pct = act_x / (double) m_win_width;
   double y_pct = act_y / (double) m_win_height;
      
   for( int aa = 0; aa < m_win_sections.size(); aa++)
   {
      WindowSection *cur = m_win_sections[aa];
      if( x_pct >= cur->GetOriginX_01() && 
          x_pct <= (cur->GetOriginX_01() + cur->GetWidth_01() ) )
      {
         if( y_pct >= cur->GetOriginY_01() &&
             y_pct <= (cur->GetOriginY_01() + cur->GetHeight_01() ) )
          {
            return cur; 
         }               
      }
    }
   return nullptr;
}
