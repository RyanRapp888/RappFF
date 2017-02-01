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
#include "TheGame.h"

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

GameMap::GameMap(int x_tiles, int y_tiles)
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

   bool GameMap::GetTileType(int x, int y, TileType &type)
   {
      int idx = GetTileIdx(x, y);
      if(idx < 0 || idx >= m_tiletypes.size())
      {
		 type = TileType::GRASS;
         return false;
      }
      
      type = m_tiletypes[idx];
      return true;
   }
   
   int GameMap::GetWorldMaxX(){return m_xtiles;}
   int GameMap::GetWorldMaxY(){return m_ytiles;}
   
    int GameMap::GetTileIdx(int xpos, int ypos)
   {
      return (xpos + (ypos * m_xtiles));
   }


DrawnCharacter::DrawnCharacter()
   {
      m_texture = nullptr;
      m_tiletype = TileType::MAINCHAR;
      TextureManager *tm = TextureManager::GetInstance();
      tm->GetTexturePtr(m_tiletype,&m_texture);
   }
   
  
   void DrawnCharacter::SetColor(RGB col){m_color = col;}
 
   void DrawnCharacter::Render()
   {
      double tile_w( GetRelativeWidth_01() * 2);
      double tile_h( GetRelativeHeight_01() * 2);
      double x_pos, y_pos;
      
      glEnable( GL_TEXTURE_2D);
      m_texture->Bind();
       
      x_pos = GetXDrawPos_N11();
      y_pos = GetYDrawPos_N11();
      
      glColor4f( m_color.GetRed()/255.0,
                 m_color.GetGreen()/255.0,
                 m_color.GetBlue()/255.0,  
                 0 ); 

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

   void Character::SetLocation(int x, int y)
   { 
      m_worldx = x; 
	  m_worldy = y;
	  if(m_worldx < 0) m_worldx = 0;
	  if(m_worldx >= m_gamemap_ptr->GetWorldMaxX()) m_worldx = m_gamemap_ptr->GetWorldMaxX() -1;
	  if(m_worldy < 0) m_worldy = 0;
	  if(m_worldy >= m_gamemap_ptr->GetWorldMaxY()) m_worldy = m_gamemap_ptr->GetWorldMaxY() -1;
   }	
   
   int Character::GetWorldX(){ return m_worldx; }
   int Character::GetWorldY(){ return m_worldy; }

   void Tile::SetColor(RGB col){m_color = col;}

   void Tile::SetTileType(TileType type)
   {
      if(type == m_tiletype && m_texture != nullptr)
      {
         return;
      }
      
      TextureManager *tm = TextureManager::GetInstance();
      tm->GetTexturePtr(type,&m_texture);
      m_tiletype = type;
   }
   
   void Tile::Render()
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
  

   void TiledGameBoard::AttachMainCharacter(Character *mc)
   { 
      m_mainchar_ptr = mc; 
	  AddObject(mc);
   }	 
  
   void TiledGameBoard::SetGameMapPtr(GameMap *gptr){ m_gamemap_ptr = gptr;}
   

   void TiledGameBoard::SetTileDetails(int xtiles, int ytiles)
   {
      m_xtiles = xtiles;
      m_ytiles = ytiles;
      
      m_tiles = new Tile[m_xtiles * m_ytiles];
            
      for(int xx = 0; xx < m_xtiles; xx++)
      {
         for(int yy = 0; yy < m_ytiles; yy++)
         {
            Tile *curtile = &(m_tiles[ GetTileIdx(xx, yy)]);
            curtile->SetColor(RGB(255,255,255));
            TileType cur;
            m_gamemap_ptr->GetTileType(xx,yy,cur);
            curtile->SetTileType(cur);
            curtile->SetRelativeLocation(xx * (1.0 / m_xtiles), yy * (1.0 / m_ytiles), 1.0 / m_xtiles, 1.0 / m_ytiles);
            AddObject(curtile);
         }         
      }
    
   }
   
   void TiledGameBoard::Refresh()
   {
      for(int xx = 0; xx < m_xtiles; xx++)
      {
         for(int yy = 0; yy < m_ytiles; yy++)
         {
            Tile *curtile = &(m_tiles[ GetTileIdx(xx, yy)]);
            curtile->SetColor(RGB(255,255,255));
            TileType cur;
            m_gamemap_ptr->GetTileType(m_mainchar_ptr->GetWorldX() - (m_xtiles / 2) + xx,
			               m_mainchar_ptr->GetWorldY() - (m_ytiles / 2) + yy,
						   cur);
            curtile->SetTileType(cur);
         }         
      }
	  	   
      int screen_mid_tilex(m_xtiles / 2);
      int screen_mid_tiley(m_ytiles / 2);	  
      Tile *center_tile = &(m_tiles[ GetTileIdx( screen_mid_tilex, screen_mid_tiley) ]);
      double ox,oy,w,h;
      center_tile->GetRelativeLocation(ox,oy,w,h);
      m_mainchar_ptr->SetRelativeLocation(ox,oy,w,h);
      WindowSection::Refresh();
   } 
      
     
   int TiledGameBoard::GetTileIdx(int xpos, int ypos)
   {
      return (xpos + (ypos * m_xtiles));
   }

  
void TheGame::KeyHandler(int key, int scancode, int action, int mods)
{
	if(key == 263) //left arrow
	{
       int xx = m_mainchar_ptr->GetWorldX();
       int yy = m_mainchar_ptr->GetWorldY();
       m_mainchar_ptr->SetLocation(xx-1,yy);
	}
	else if(key == 262)
	{
	   int xx = m_mainchar_ptr->GetWorldX();
       int yy = m_mainchar_ptr->GetWorldY();
       m_mainchar_ptr->SetLocation(xx+1,yy);
	}
	else if(key == 265)
	{
	   int xx = m_mainchar_ptr->GetWorldX();
       int yy = m_mainchar_ptr->GetWorldY();
       m_mainchar_ptr->SetLocation(xx,yy+1);
	}
	else if(key == 264)
	{
	   int xx = m_mainchar_ptr->GetWorldX();
       int yy = m_mainchar_ptr->GetWorldY();
       m_mainchar_ptr->SetLocation(xx,yy-1);
	}
	std::cout << "key = " << key << std::endl;
}
	
void TheGame::Play()
{
	   m_display_ptr = new Display(800, 600, "Cupcake");
	   if(m_display_ptr == nullptr) return;
	   SetEvilPtr(this);
   
       /*
	   Tile *lower = new Tile();
       lower->SetTileType(TileType::PARCHMENT);
       lower->SetRelativeLocation(.01,.01,.98,.28);
       lower->SetColor(RGB(255,255,255));
       display.AddObject(lower);
       */
	   //ultimately, I want gamemap to be 256x256
	   m_gamemap_ptr = new GameMap(40,30);
       TiledGameBoard *upper = new TiledGameBoard(m_display_ptr,.01,.01,.98,.98,m_gamemap_ptr);
       upper->SetTileDetails(16,16);
       m_display_ptr->AddWindowSection(upper);
	   m_mainchar_ptr = new Character("main_character", CharacterType::CAT, CharMotion(),
             20, 20, m_gamemap_ptr);
	   m_mainchar_ptr->SetColor(RGB(255,255,255));
	   upper->AttachMainCharacter(m_mainchar_ptr);
				
       while(!m_display_ptr->WindowShouldClose())
       {
          m_display_ptr->Clear(0,0,.2,1);
          m_display_ptr->Refresh();
          m_display_ptr->SwapBuffers();     
          glfwPollEvents();
	   }
   }

 int main(void)
{
   
   glfwSetErrorCallback(error_callback);
      
   if (!glfwInit())
   {
      exit(EXIT_FAILURE);
   }
   
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   
   TheGame game;
   game.Play();
   
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
