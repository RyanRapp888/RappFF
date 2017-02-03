#ifndef DRAWABLEOBJ_H__
#define DRAWABLEOBJ_H__

#include "WindowSection.h"
#include "Viewport.h"

class DrawableObj
{
  friend class WindowSection;
  friend class Viewport;

public:

	DrawableObj():m_rel_originx_01(0),
                 m_rel_originy_01(0),
                 m_rel_width_01(0),
                 m_rel_height_01(0),
                 m_winsection_ptr(nullptr),
                 m_viewport_ptr(nullptr){}

	DrawableObj &operator=(const DrawableObj & other);

   virtual ~DrawableObj(){}
  
   // This function is meant only for copying the location of two objects
   // in the same winsection or viewport 
   void GetRelativeLocation(
      double &originx_01, double &originy_01,
      double &width_01,double &height_01);
     
   void SetRelativeLocation(
      double originx_01,double originy_01,
      double width_01,double height_01);

   virtual void Render() = 0;

   
    
    void SetWindowSectionPtr(WindowSection *obj)
	{
		m_winsection_ptr = obj;
	} 

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
  
   // DO NOT FREE THESE, owned by parent
   WindowSection *m_winsection_ptr;
   Viewport *m_viewport_ptr;

};

#endif
