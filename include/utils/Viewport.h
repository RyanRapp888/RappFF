#ifndef VIEWPORT_H__
#define VIEWPORT_H__

#include <vector>

class WindowSection;
class DrawableObj;

class Viewport
{

public:
  
   virtual ~Viewport(){} 
   void SetWindowSize(int win_w, int win_h);
   virtual bool Refresh();
   void AddWindowSection( WindowSection* dat);
   void RemoveWindowSection(WindowSection *dat);
   void AddObject(DrawableObj *dat);
   void RemoveObject(DrawableObj *dat);
   int GetWinWidth();
   int GetWinHeight();
 
   protected:
   
   WindowSection *FindWindowSection(int x, int y);
   int m_win_width;
   int m_win_height;
    
   std::vector< WindowSection* > m_win_sections;
   std::vector< DrawableObj* > m_drawables;
};

#endif
