#ifndef TILE_H__
#define TILE_H__

#include "DrawableObj.h"
#include "Texture.h"
#include "RGB.h"

class Tile: public DrawableObj
{

public:
   
   Tile()
   {
      m_texture = nullptr;
      m_tiletype = TileType::GRASS;
   }
   void SetColor(RGB col);
   void SetTileType(TileType type);
   void Render();
      
   private:

   Texture *m_texture;
   TileType m_tiletype;   
   RGB m_color;
};

#endif
