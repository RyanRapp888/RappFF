#ifndef WINDOWSECTION_H__
#define WINDOWSECTION_H__

#include <vector>
#include "DrawableObj.h"
#include "Viewport.h"

class WindowSection
{
	
	friend class Viewport;

public:
	WindowSection();
  
   WindowSection(Viewport *vpt, double origxpct, double origypct, 
                 double w_pct, double h_pct):m_view_ptr(vpt),
                                             m_originx_01(origxpct),
                                             m_originy_01(origypct),
                                             m_width_01(w_pct),
                                             m_height_01(h_pct){}
   virtual ~WindowSection(){}
   virtual void Refresh();
   double GetOriginX_01();     
   double GetOriginY_01();
   double GetWidth_01();
   double GetHeight_01();
   void Enable();
   void Disable();
   void AddObject( DrawableObj *dat);
   void RemoveObject( DrawableObj *dat);
   Viewport *GetVP();

protected:

	bool m_enable;

private:

   Viewport *m_view_ptr;
   // m_originx & m_originy are relative to the viewport
   double m_originx_01, m_originy_01;
   double m_width_01, m_height_01;
   std::vector< DrawableObj* > m_drawables;
   
};

#endif