#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include <utils/WinViewportUtils.h>
#include "Windows.h"
#include <stdlib.h>
#include <map>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <fstream>
#include <sstream>

static void PrintGLParams();

static void error_callback(int error, const char* description)
{
   fprintf(stderr, "Error: %s\n", description);
}

int get_rand()
{
   static bool is_seeded(false);
   if(!is_seeded) 
   {
      srand(14);
      is_seeded = true;
   }
   int result = rand() % 7;
   return result;
}

class GameMap
{
   public:

   GameMap(int x_tiles, int y_tiles)
   {
      m_xtiles = x_tiles;
      m_ytiles = y_tiles;
      m_tiletypes.resize(x_tiles * y_tiles);

      std::ifstream input_file("../../res/map.csv");

      for(int row = y_tiles - 1; row >= 0; row--)
      {
         std::string line;
         std::getline(input_file, line);
         if( !input_file.good() )
            break;

         std::stringstream iss(line);

         for  (int col = 0; col < x_tiles; col++)
         {
            std::string val;
            std::getline(iss, val, ',');
            if(!iss.good() )
               break;

            std::stringstream tmp_sstr(val);
            int tmpint; 
            tmp_sstr >> tmpint;
            m_tiletypes[ GetTileIdx( col, row) ] = static_cast<TileType>(tmpint);
         }
      }
      input_file.close();
   }

   bool GetTileType(int x, int y, TileType &type)
   {
      int idx = GetTileIdx(x, y);
      if(idx < 0 || idx >= m_tiletypes.size())
      {
         return false;
      }
      
      type = m_tiletypes[idx];
      return true;
   }

   private:
   
   int m_xtiles;
   int m_ytiles;
   std::vector<TileType> m_tiletypes;
   int GetTileIdx(int xpos, int ypos)
   {
      return (xpos + (ypos * m_xtiles));
   }
};

class Character: public DrawableObj
{
   public:
   Character()
   {
      m_texture = nullptr;
      m_tiletype = TileType::MAINCHAR;
      TextureManager *tm = TextureManager::GetInstance();
      tm->GetTexturePtr(m_tiletype,&m_texture);
   }

   void SetColor(RGB col){m_color = col;}
 
   void Render()
   {
      double tile_w( GetRelativeWidth_01() * 2);
      double tile_h( GetRelativeHeight_01() * 2);
      double x_pos, y_pos;
      
      glEnable( GL_TEXTURE_2D);
      m_texture->Bind();
       
      x_pos = GetXDrawPos_N11();
      y_pos = GetYDrawPos_N11();
      
      glColor3f( m_color.GetRed()/255.0,
                 m_color.GetGreen()/255.0,
                 m_color.GetBlue()/255.0 ); 

      glBegin(GL_TRIANGLES);
      glTexCoord2d( 0.0, 0.0); glVertex3f( x_pos, y_pos, -.01);
      glTexCoord2d( 1.0, 0.0); glVertex3f( x_pos + tile_w, y_pos, -0.01);  
      glTexCoord2d( 1.0, 1.0); glVertex3f( x_pos + tile_w, y_pos + tile_h, -0.01); 
      glTexCoord2d( 1.0, 1.0); glVertex3f( x_pos + tile_w, y_pos + tile_h,-0.01);
      glTexCoord2d( 0.0, 1.0); glVertex3f( x_pos, y_pos + tile_h,-0.01);
      glTexCoord2d( 0.0, 0.0); glVertex3f( x_pos, y_pos, -0.01);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, 0);
      Sleep(100);
   }
   private:
    Texture *m_texture;
    TileType m_tiletype;   
    RGB m_color;
};

class Tile: public DrawableObj
{

public:
   
   Tile()
   {
      m_texture = nullptr;
      m_tiletype = TileType::GRASS;
   }

   void SetColor(RGB col){m_color = col;}

   void SetTileType(TileType type)
   {
      if(type == m_tiletype && m_texture != nullptr)
      {
         return;
      }
      
      TextureManager *tm = TextureManager::GetInstance();
      tm->GetTexturePtr(type,&m_texture);
      m_tiletype = type;
   }
   
   void Render()
   {
      double tile_w(  GetRelativeWidth_01() * 2);
      double tile_h( GetRelativeHeight_01() * 2);
      double x_pos, y_pos;
      
      glEnable( GL_TEXTURE_2D);
      m_texture->Bind();
       
      x_pos = GetXDrawPos_N11();
      y_pos = GetYDrawPos_N11();
      
      glColor3f( m_color.GetRed()/255.0,
                 m_color.GetGreen()/255.0,
                 m_color.GetBlue()/255.0 ); 

      glBegin(GL_TRIANGLES);
      glTexCoord2d( 0.0, 0.0); glVertex3f( x_pos, y_pos, 0);
      glTexCoord2d( 1.0, 0.0); glVertex3f( x_pos + tile_w, y_pos, 0);  
      glTexCoord2d( 1.0, 1.0); glVertex3f( x_pos + tile_w, y_pos + tile_h, 0); 
      glTexCoord2d( 1.0, 1.0); glVertex3f( x_pos + tile_w, y_pos + tile_h, 0);
      glTexCoord2d( 0.0, 1.0); glVertex3f( x_pos, y_pos + tile_h, 0);
      glTexCoord2d( 0.0, 0.0); glVertex3f( x_pos, y_pos, 0);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, 0);
   }
   
   private:

   Texture *m_texture;
   TileType m_tiletype;   
   RGB m_color;
};



class TiledGameBoard: public WindowSection
{
   public:
   TiledGameBoard():m_tiles(nullptr),m_gamemap(nullptr){}
   TiledGameBoard(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct):
      WindowSection(vpt,origxpct,origypct,w_pct,h_pct),m_gamemap(nullptr){}
   ~TiledGameBoard(){ delete [] m_tiles; m_tiles = nullptr; }

   void SetTileDetails(int xtiles, int ytiles)
   {
      m_xtiles = xtiles;
      m_ytiles = ytiles;
      
      m_tiles = new Tile[m_xtiles * m_ytiles];
      m_gamemap = new GameMap(m_xtiles, m_ytiles);
      
      for(int xx = 0; xx < m_xtiles; xx++)
      {
         for(int yy = 0; yy < m_ytiles; yy++)
         {
            Tile *curtile = &(m_tiles[ GetTileIdx(xx, yy)]);
            curtile->SetColor(RGB(255,255,255));
            TileType cur;
            m_gamemap->GetTileType(xx,yy,cur);
            curtile->SetTileType(cur);
            curtile->SetRelativeLocation(xx * (1.0 / m_xtiles), yy * (1.0 / m_ytiles), 1.0 / m_xtiles, 1.0 / m_ytiles);
            curtile->SetParentWindowSection(this);
            AddObject(curtile);
         }         
      }

      m_mainchar.SetColor(RGB(255,255,255));
      m_mainchar.SetParentWindowSection(this);
      AddObject(&m_mainchar);
   }
   
   void Refresh()
   {
      static int cur_column(0);
      Tile *charloc = &(m_tiles[ GetTileIdx( cur_column, 10 )]);
      double ox,oy,w,h;
      charloc->GetRelativeLocation(ox,oy,w,h);
      m_mainchar.SetRelativeLocation(ox,oy,w,h);
      cur_column++;
      if(cur_column >= m_xtiles) cur_column = 0;
      WindowSection::Refresh();
   } 
      
   private:
   
   int GetTileIdx(int xpos, int ypos)
   {
      return (xpos + (ypos * m_xtiles));
   }

   Character m_mainchar;
   int m_xtiles;
   int m_ytiles;
   Tile *m_tiles;
   GameMap *m_gamemap;
};

int main(void)
{
   
   glfwSetErrorCallback(error_callback);
      
   if (!glfwInit())
   {
      exit(EXIT_FAILURE);
   }
   
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   
   Display display(650, 650, "Rapp It Up");

   Tile *lower = new Tile();
   lower->SetTileType(TileType::PARCHMENT);
   lower->SetRelativeLocation(.01,.01,.98,.28);
   lower->SetParentViewport(&display);
   lower->SetColor(RGB(255,255,255));
   display.AddObject(lower);

   TiledGameBoard *upper = new TiledGameBoard(&display,.01,.3,.98,.69);
   upper->SetTileDetails(40,30);
   display.AddWindowSection(upper);

   while(!display.WindowShouldClose())
   {
      display.Clear(0,0,.2,1);
      display.Refresh();
      
      display.SwapBuffers();     
      glfwPollEvents();
   }

   return 0;
}

void PrintGLParams()
{
   GLenum params[] = {
      GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
      GL_MAX_CUBE_MAP_TEXTURE_SIZE,
      GL_MAX_DRAW_BUFFERS,
      GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
      GL_MAX_TEXTURE_IMAGE_UNITS,
      GL_MAX_TEXTURE_SIZE,
      GL_MAX_VARYING_FLOATS,
      GL_MAX_VERTEX_ATTRIBS,
      GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
      GL_MAX_VERTEX_UNIFORM_COMPONENTS,
      GL_MAX_VIEWPORT_DIMS,
      GL_STEREO,
   };

   const char *names[] = {
      "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
      "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
      "GL_MAX_DRAW_BUFFERS",
      "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
      "GL_MAX_TEXTURE_IMAGE_UNITS",
      "GL_MAX_TEXTURE_SIZE",
      "GL_MAX_VARYING_FLOATS",
      "GL_MAX_VERTEX_ATTRIBS",
      "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
      "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
      "GL_MAX_VIEWPORT_DIMS",
      "GL_STEREO",
   };
   
   std::cout << "GL Context Params:\n";
   // integers - only works if the order is 0-10 integer return types
   for ( int i = 0; i < 10; i++ )
   {
      int v = 0;
      glGetIntegerv( params[i], &v );
      std::cout << names[i] << " = " << v << std::endl;
   }
   
   int v[2];
   v[0] = v[1] = 0;
   glGetIntegerv( params[10], v );
   printf( "%s %i %i\n", names[10], v[0], v[1] );
   unsigned char s = 0;
   glGetBooleanv( params[11], &s );
   printf( "%s %i\n", names[11], (unsigned int)s );
   printf( "-----------------------------\n" );
}   
