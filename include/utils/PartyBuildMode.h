#ifndef PARTYBUILD_H__
#define PARTYBUILD_H__

#include "WindowSection.h"
#include "Tile.h"
#include "Text.h"

enum class PartyBuildSelectType
{
	CHARACTER_CHOOSE,
	TYPE_NAME
};

class PartyBuildMode : public WindowSection
{

public:

	PartyBuildMode(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct);
	~PartyBuildMode();
	bool SetTextHandler(Text *texthandler);
	bool SetPrimaryShader(GLuint primary_shader);
	void Refresh();
	bool HandleKey(int key, int scancode, int action, int mods);
	bool IsPartyBuildComplete() const;

private:

	PartyBuildSelectType m_select_type;
	void DrawPartyBuildWindow();
	bool m_build_complete;
	int m_cur_hero_idx;
	Tile m_drawparty_tile;
	Text *m_texthandler_ptr;
	GLuint m_primary_shaderid;
	
	int m_heros_to_build;
	std::vector<std::string> m_charnames;
	Tile *m_char_build_tiles;
};

#endif
