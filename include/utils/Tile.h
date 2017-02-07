#ifndef TILE_H__
#define TILE_H__

#include "DrawableObj.h"
#include "Texture.h"
#include "RGB.h"
#include "Mesh.h"

class Tile: public DrawableObj
{
public:
   
   Tile()
   {
      m_mesh_ptr = nullptr;
	  m_tiletype = TileType::GRASS;
   }

   void SetColor(RGB col);
   void SetTileType(TileType type);
   void Render();
   Mesh *GetMeshPtr();
      
   private:

   Mesh *m_mesh_ptr;
   TileType m_tiletype;   
   RGB m_color;
};

#endif
