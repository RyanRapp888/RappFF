#include "FightScreen.h"

FightScreen::FightScreen(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct) :
	WindowSection(vpt, origxpct, origypct, w_pct, h_pct)
{
	Tile *bltile = &(m_five_window_parts[0]);
	bltile->SetTileType(TileType::PARCHMENT);
	bltile->SetWindowSectionPtr(this);
	bltile->SetRelativeLocation(0, 0, .357, .35);
	//bltile->SetRelativeLocation(0, 0, 1, .5);
	

	Tile *tltile = &(m_five_window_parts[1]);
	tltile->SetWindowSectionPtr(this);
	tltile->SetTileType(TileType::BRICKS);
	tltile->SetRelativeLocation(0, .35, .535, .65);
    //tltile->SetRelativeLocation(0, .5, .5, .5);

	
	Tile *brtile = &(m_five_window_parts[2]);
	brtile->SetWindowSectionPtr(this);
	brtile->SetTileType(TileType::DESERT);
	brtile->SetRelativeLocation(.357, 0, .428, .35);
	//brtile->SetRelativeLocation(.5, 0, .5, .5);

	Tile *trtile = &(m_five_window_parts[3]);
	trtile->SetWindowSectionPtr(this);
	trtile->SetTileType(TileType::MUD);
	trtile->SetRelativeLocation(.535, .35, .264, .65);
	//trtile->SetRelativeLocation(.5, .5, .5, .5);
	
	Tile *frtile = &(m_five_window_parts[4]);
	frtile->SetWindowSectionPtr(this);
	frtile->SetTileType(TileType::PLANK);
	frtile->SetRelativeLocation(.785, 0, .214, 1);
	
	m_enable = false;
	
}
	

void FightScreen::Refresh()
{
	if (!m_enable) return;
		
	for (int aa = 0; aa < 5; aa++)
	{
		Tile *cur = &(m_five_window_parts[aa]);
		int n_instances = 1;
		glm::mat4 *translations = new glm::mat4[n_instances];
		glm::vec3 scalevec(cur->GetRelativeWidth_01(), cur->GetRelativeHeight_01(), 1);
		float floatscales[3];
		floatscales[0] = cur->GetXDrawPos_N11() - (1 - scalevec.x);
		floatscales[1] = cur->GetYDrawPos_N11() - (1 - scalevec.y);
		floatscales[2] = 0;

		glm::vec3 transvec(floatscales[0],floatscales[1],floatscales[2]);
 		translations[0] = glm::translate(glm::mat4(1.0), transvec);
		scalevec *= 2;
		cur->SetUpInstancing(1, scalevec, translations);
		cur->Render();
		delete[] translations;
	}

	WindowSection::Refresh();
}