#ifndef NEWORLOADMODE_H__
#define NEWORLOADMODE_H__

#include "WindowSection.h"
#include "Tile.h"
#include "Text.h"

enum class NOL_PICK_MODE
{
	NEWORLOAD_PICK,
	CHOOSE_LOADABLE
};

class NewOrLoadMode : public WindowSection
{
public:
	NewOrLoadMode(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct);
	~NewOrLoadMode() {}
	bool SetTextHandler(Text *texthandler);
	bool SetPrimaryShader(GLuint primary_shader);
	void Refresh();
	bool HandleKey(int key, int scancode, int action, int mods);
	std::string GetSaveFileName();

private:

	NOL_PICK_MODE m_select_type;
	void DrawMainScreen();
	bool m_enable;
	bool m_screen_complete;
	int m_opt_idx;
	Tile m_neworload_tile;
	Text *m_texthandler_ptr;
	GLuint m_primary_shaderid;
	std::string m_filename;
	std::vector<std::string> m_cur_options;

};

#endif
