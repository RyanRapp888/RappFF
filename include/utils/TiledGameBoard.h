#ifndef TILEDGAMEBOARD_H__
#define TILEDGAMEBOARD_H__

#include "Character.h"
#include "WindowSection.h"
#include "GameMap.h"
#include "DrawnCharacter.h"
#include "Tile.h"


class TiledGameBoard: public WindowSection
{
   public:
      
   TiledGameBoard(Viewport *vpt, double origxpct, double origypct, double w_pct, 
                  double h_pct, GameMap *gamemap_ptr):
         WindowSection(vpt,origxpct,origypct,w_pct,h_pct),m_tiles(nullptr),m_gamemap_ptr(gamemap_ptr){}
   
   
   ~TiledGameBoard(){ delete [] m_tiles; m_tiles = nullptr; }
   void SetGameMapPtr(GameMap *gptr);
   void SetTileDetails(int xtiles, int ytiles);
   void Refresh();
   
   private:
   


   TiledGameBoard():m_tiles(nullptr),m_gamemap_ptr(nullptr){}
   int GetTileIdx(int xpos, int ypos);
   void Print();

   int m_xtiles;
   int m_ytiles;
   Tile *m_tiles;
   GameMap *m_gamemap_ptr;
   glm::vec3 m_scale_vec;
};

#endif
