#ifndef WINVIEWPORTUTILS__H
#define WINVIEWPORTUTILS__H

#include <iostream>
#include <utils/RGB.h>
#include <vector>
#include <algorithm>

class WindowSection;
class Viewport;

class DrawableObj
{
   public:
      DrawableObj():m_rel_originx_01(0),m_rel_originy_01(0),m_rel_width_01(0),m_rel_height_01(0),
	       m_winsection(nullptr),m_viewport(nullptr){}
  
     // This function is meant only for copying the location of two objects
     // in the same winsection or viewport 
     void GetRelativeLocation(
      double &originx_01, double &originy_01,
	  double &width_01,double &height_01);
	  
	  void SetParentViewport(Viewport *vp);
	  void SetParentWindowSection(WindowSection *ws);
      void SetRelativeLocation(double originx_01,double originy_01,
	                           double width_01,double height_01);
	  virtual void Render() = 0;
	  virtual void Print();
	  
   protected:
   
      double GetRelativeOriginX_01();
	  double GetRelativeOriginY_01();
      double GetRelativeWidth_01();
	  double GetRelativeHeight_01();
	  double GetXDrawPos_N11();
	  double GetYDrawPos_N11();
      
      // if not using absolute coords
	  // then we are drawing relative to a window or viewport
      double m_rel_originx_01;
	  double m_rel_originy_01;
	  double m_rel_width_01;
      double m_rel_height_01;
      WindowSection *m_winsection;
      Viewport *m_viewport;
};

class WindowSection
{
   public:
      WindowSection():m_view_ptr(nullptr),m_originx_01(0),m_originy_01(0),m_width_01(0),m_height_01(0){}
      WindowSection(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct):
	     m_view_ptr(vpt),m_originx_01(origxpct),m_originy_01(origypct),m_width_01(w_pct),m_height_01(h_pct)
	  {}
	  
	  virtual void Refresh();
	  double GetOriginX_01();	  
	  double GetOriginY_01();
	  double GetWidth_01();
	  double GetHeight_01();
	  
	  void AddObject(DrawableObj *dat);
	  void RemoveObject(DrawableObj *dat);
	  void FreeDrawables();
	  Viewport *GetVP();
	  

   private:
      Viewport *m_view_ptr;
	  // m_originx & m_originy are relative the viewport
      double m_originx_01, m_originy_01;
	  double m_width_01, m_height_01;
	  std::vector<DrawableObj *> m_drawables;  
};

class Viewport
{
   public:
   
   void SetWindowSize(int win_w, int win_h);
   virtual bool Refresh();
   void AddWindowSection(WindowSection *dat);
   void RemoveWindowSection(WindowSection *dat);
   void AddObject(DrawableObj *dat);
   void RemoveObject(DrawableObj *dat);
   int GetWinWidth();
   int GetWinHeight();
 
   protected:
   
   WindowSection *FindWindowSection(int x, int y);
   void FreeDrawables();
   void FreeWindowSections();
   int m_win_width;
   int m_win_height;
    
   std::vector<WindowSection *> m_win_sections;
   std::vector<DrawableObj *> m_drawables;
};

#endif
