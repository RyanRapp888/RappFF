#ifndef THEGAME_H
#define THEGAME_H

#include <display.h>
#include <vector>
#include <map>
#include <texture.h>
#include <utils/RGB.h>
#include <utils/WinViewportUtils.h>


class GameMap;
class TheGame;
class Display;
class Character;
class DrawableCharacter;


class GameMap
{
   public:

   GameMap(int x_tiles, int y_tiles);
   bool GetTileType(int x, int y, TileType &type);
   int GetWorldMaxX();
   int GetWorldMaxY();
   
   private:
   
   int m_xtiles;
   int m_ytiles;
   std::vector<TileType> m_tiletypes;
   int GetTileIdx(int xpos, int ypos);
};

class DrawnCharacter: public DrawableObj
{
   public:
   DrawnCharacter();
   virtual ~DrawnCharacter(){}
  //we may want to eventually do reference counting on the texture
   void SetColor(RGB col);
   void Render();
   
   private:
    Texture *m_texture;
    TileType m_tiletype;   
    RGB m_color;
};

class TheGame
{
	public:
	TheGame(){}
	void KeyHandler(int key, int scancode, int action, int mods);
	void Play();
	~TheGame()
    {
      delete m_display_ptr; m_display_ptr = nullptr;
	  delete m_gamemap_ptr; m_gamemap_ptr = nullptr;
    }
   
   private:
   
   GameMap *m_gamemap_ptr;
   Character *m_mainchar_ptr;
   Display *m_display_ptr;
};

enum class CharacterType
{
   CAT, SOLDIER, ROBOT, CUPCAKE, NURSE, HORSE, N_CHARTYPES
};

enum class CharMotionType
{
	USER, STATIC, RANDOAREA, PATHREPEATER
};

class CharMotion
{
	public:
	CharMotion():m_charmotion_type(CharMotionType::STATIC){}
	CharMotion(CharMotionType t):m_charmotion_type(t){}
	private:
	CharMotionType m_charmotion_type;
	// xy coord range for area to wander
	// xy coords for path to follow, etc
};

class Character: public DrawnCharacter
{
   public:
   Character(const std::string &name, CharacterType ctype, CharMotion cmot,
             int worldx, int worldy, GameMap *gamemap_ptr): 
			    m_name(name), m_char_type(ctype), m_char_motion(cmot),
				m_worldx(worldx), m_worldy(worldy), m_gamemap_ptr(gamemap_ptr){}
   void SetLocation(int x, int y);
   int GetWorldX();
   int GetWorldY();
   private:
   Character(){}
   std::string m_name;
   CharacterType m_char_type;
   CharMotion m_char_motion;
   GameMap *m_gamemap_ptr;
   int m_worldx, m_worldy;
};

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

class TiledGameBoard: public WindowSection
{
   public:
      
   TiledGameBoard(Viewport *vpt, double origxpct, double origypct, double w_pct, 
                  double h_pct, GameMap *gamemap_ptr):
         WindowSection(vpt,origxpct,origypct,w_pct,h_pct),m_gamemap_ptr(gamemap_ptr){}
   void AttachMainCharacter(Character *mc);
   ~TiledGameBoard(){ delete [] m_tiles; m_tiles = nullptr; }
   void SetGameMapPtr(GameMap *gptr);
   void SetTileDetails(int xtiles, int ytiles);
   void Refresh();
   
   private:
   
   TiledGameBoard():m_tiles(nullptr),m_gamemap_ptr(nullptr){}
   int GetTileIdx(int xpos, int ypos);
  
   Character *m_mainchar_ptr;
   // maybe have a list of pointers to other character that "TheGame" knows about.
   int m_xtiles;
   int m_ytiles;
   Tile *m_tiles;
   GameMap *m_gamemap_ptr;
};
#endif