#ifndef TILE_H__
#define TILE_H__

#include "DrawableObj.h"
#include "Texture.h"
#include "RGB.h"
#include "Mesh.h"

class Tile: public Mesh
{
public:
   
   Tile()
   {
      m_mesh_ptr = nullptr;
	  m_tiletype = TileType::GRASS;
	  m_color = RGB(255, 255, 255);
   }

   void SetColor(RGB col);
   void SetTileType(TileType type);
   TileType GetTileType();
   void Render();
   Mesh *GetMeshPtr();
      
   private:

   TileType m_tiletype;
   Mesh *m_mesh_ptr;
   RGB m_color;
};

#endif
