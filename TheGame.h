#ifndef THEGAME_H__
#define THEGAME_H__

#include "GameMap.h"
#include "Character.h"
#include "Display.h"
#include "FightMode.h"
#include "TiledGameBoard.h"
#include "PartyBuildMode.h"
#include "NewOrLoadMode.h"

enum class GameMode
{
	MapWalkingMode,
	FightMode,
	PartyBuildMode,
    NewOrLoadMode
};

class TheGame
{
public:
	TheGame();
	void KeyHandler(int key, int scancode, int action, int mods);
	void Play();
	~TheGame()
	{
		//clean all the stuff
	}
	bool LoadGameFromFile(const std::string &filename);
	bool LoadGameFromPartyBuild();
	bool SaveGameToFile(const std::string &filename) const;
	
private:
	void SetCurMode(GameMode dat);
	void Interact(int x, int y);
		
	std::vector<Character *> m_chars;
	
	Display *m_display_ptr;
	TiledGameBoard *m_mapwalkingmode_ws;
	FightMode *m_fightmode_ws;
	PartyBuildMode *m_partybuild_ws;
	NewOrLoadMode *m_neworload_ws;
	GameMode m_cur_mode;
	int m_xrot, m_yrot, m_zrot;
	
	bool m_chatting;
	Character *m_chat_char;
	ProxRel m_chat_prox;
};

#endif
