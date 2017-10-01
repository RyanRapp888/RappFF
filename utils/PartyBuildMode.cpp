#include "PartyBuildMode.h"
#include "Character.h"
#include <glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include "RandUtils.h"

PartyBuildMode::PartyBuildMode(Viewport *vpt, double origxpct, double origypct, double w_pct, double h_pct) :
WindowSection(vpt, origxpct, origypct, w_pct, h_pct), m_texthandler_ptr(nullptr), m_primary_shaderid(0)
{
	m_drawparty_tile.SetWindowSectionPtr(this);
	m_drawparty_tile.SetTileType(TileType::FRAME);
	m_drawparty_tile.SetRelativeLocation(0, 0, 1, 1);
	m_enable = false;
	m_build_complete = false;
	m_cur_choice_idx = 0;
	m_heros_to_build = 4;
	m_select_type = PartyBuildSelectType::OPTION_CHOOSE;
	m_save_name = "Save1";
	m_charnames = { "Player 1", "Player 2", "Player 3", "Player 4" };
   m_char_build_tiles = new Tile[m_heros_to_build];
	for (int aa = 0; aa < m_heros_to_build; aa++)
	{
		m_char_build_tiles[aa].SetTileType(TileType::MERMAID);
	}
}

PartyBuildMode::~PartyBuildMode()
{
	delete[] m_char_build_tiles;
}

bool PartyBuildMode::SetTextHandler(Text *texthandler)
{
	m_texthandler_ptr = texthandler;
	return true;
}

bool PartyBuildMode::SetPrimaryShader(GLuint primary_shader)
{
	m_primary_shaderid = primary_shader;
	return true;
}

void PartyBuildMode::Refresh()
{
	if (!m_enable) return;

	DrawPartyBuildWindow();
	
	WindowSection::Refresh();
}

bool PartyBuildMode::IsPartyBuildComplete() const
{
	return m_build_complete;
}

bool PartyBuildMode::HandleKey(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (m_select_type == PartyBuildSelectType::TYPE_NAME &&
			((key >= GLFW_KEY_0 && key <= GLFW_KEY_9) ||
			(key >= GLFW_KEY_A && key <= GLFW_KEY_Z)))
		{
			if (m_cur_choice_idx == 0)
			{
				// user is adding a letter to their save name
				if (m_save_name.size() <= 9)
				{
				   const char userkey(static_cast<char>(key));
				   m_save_name.push_back(userkey);
				}
			}
			else if (m_charnames[m_cur_choice_idx-1].size() <= 9)
			{
			   const char userkey(static_cast<char>(key));
			   m_charnames[m_cur_choice_idx-1].push_back(userkey);
			}
		}
		else if (m_select_type == PartyBuildSelectType::TYPE_NAME &&
			key == GLFW_KEY_BACKSPACE)
		{
			if (m_cur_choice_idx == 0)
			{
				// user is removing a letter from their save name
				if (m_save_name.size() > 0)
				{
					m_save_name.pop_back();
				}
			}
			else if (m_charnames[m_cur_choice_idx-1].size() > 0)
			{
				m_charnames[m_cur_choice_idx-1].pop_back();
			}
		}
		else if (key == GLFW_KEY_DOWN)
		{
			if (m_select_type == PartyBuildSelectType::OPTION_CHOOSE)
			{
				m_cur_choice_idx++;
				// the +2 is for the "Save Name" and "Done" options
				if (m_cur_choice_idx >= m_heros_to_build + 2) m_cur_choice_idx = m_heros_to_build;
			}
		}
		else if (key == GLFW_KEY_UP)
		{
			if (m_select_type == PartyBuildSelectType::OPTION_CHOOSE)
			{
				m_cur_choice_idx--;
				if (m_cur_choice_idx < 0) m_cur_choice_idx = 0;
			}
		}
		else if (key == GLFW_KEY_ENTER)
		{
			if (m_select_type == PartyBuildSelectType::OPTION_CHOOSE)
			{
				if (m_cur_choice_idx == m_heros_to_build + 1)
				{
					ValidateAndFixSave();
					m_build_complete = true;
				}
				else
				{
					m_select_type = PartyBuildSelectType::TYPE_NAME;
				}
			}
			else if (m_select_type == PartyBuildSelectType::TYPE_NAME)
			{
				m_select_type = PartyBuildSelectType::OPTION_CHOOSE;
				if (m_cur_choice_idx == (m_heros_to_build + 1))
				{
					ValidateAndFixSave();
					m_build_complete = true;
				}
				else if (m_cur_choice_idx <= m_heros_to_build)
				{
					m_cur_choice_idx++;
				}
	    	}
		}
	}
	return true;
}

std::string PartyBuildMode::GetSaveLoc() const
{
	return "\\res\\" + m_save_name + m_save_file_ext;
}

void  PartyBuildMode::ValidateAndFixSave()
{
	// The arrow is on the "Done" option
	for (auto cur_hero : m_charnames)
	{
		if (cur_hero.size() == 0)
		{
			cur_hero = "Blanky";
		}
		else if (cur_hero.size() > 10)
		{
			cur_hero = cur_hero.substr(0, 10);
		}
	}

	std::vector<std::string> pre_existing_save_list;
	get_files("res", "toc", pre_existing_save_list);
	
	std::string save_folder("res\\");
	std::string proposed_save_loc(save_folder);
	proposed_save_loc += m_save_name + m_save_file_ext;
	std::string uppercase_save_loc = to_upper(proposed_save_loc);
	
	for (auto cur_prexisting : pre_existing_save_list)
	{
		std::string prexisting = to_upper(cur_prexisting);
		int num = 1;
		while (prexisting == uppercase_save_loc)
		{
			proposed_save_loc = save_folder;
			m_save_name += to_string(num);
			num++;
			proposed_save_loc += m_save_name + m_save_file_ext;
			uppercase_save_loc = to_upper(proposed_save_loc);
			if (num > 100) break;
		}
	}
}

void PartyBuildMode::DrawPartyBuildWindow()
{
	glUseProgram(m_primary_shaderid);

	int n_instances = 1;
	glm::mat4 *translations = new glm::mat4[n_instances];

	double ox = m_drawparty_tile.GetRelativeOriginX_01();
	double oy = m_drawparty_tile.GetRelativeOriginY_01();
	double xd = m_drawparty_tile.GetRelativeWidth_01();
	double yd = m_drawparty_tile.GetRelativeHeight_01();
	double mx = ox + (xd / 2.0);
	double my = oy + (yd / 2.0);
	double mx2 = mx - .5;
	double my2 = my - .5;
	glm::vec3 scalevec(m_drawparty_tile.GetRelativeWidth_01() * 2,
		m_drawparty_tile.GetRelativeHeight_01() * 2, 1);
	mx2 *= scalevec.x;
	my2 *= scalevec.y;
	glm::vec3 transvec(2 * mx2, 2 * my2, 0);
	translations[0] = glm::translate(glm::mat4(1.0), transvec);
	m_drawparty_tile.SetUpInstancing(1, scalevec, translations);
	m_drawparty_tile.Render();
	delete[] translations;

	glm::vec3 char_scalevec(m_drawparty_tile.GetRelativeWidth_01() * 2,
		m_drawparty_tile.GetRelativeHeight_01() * 2, 1);
	char_scalevec.x *= .1f;
	char_scalevec.y *= .1f;

	int n_choices = m_heros_to_build + 2; // the +2 is for the "Save name" and "Done" options
	float n_choice_height = 1.0 / (n_choices+2); // +1 here is to add a little space between the text choices
	
	for (int aa = 0; aa < n_choices; aa++)
	{
		float xdrawpos = static_cast<float>(m_drawparty_tile.GetXDrawPos_N11());
		float ydrawpos = static_cast<float>(m_drawparty_tile.GetYDrawPos_N11());
		float charoriginx = xdrawpos + (static_cast<float>(m_drawparty_tile.GetRelativeWidth_01()) * 2.0f) * .15f;
		float charoriginy = ydrawpos + (static_cast<float>(m_drawparty_tile.GetRelativeHeight_01()) * 2.0f) * (.83f - (aa * n_choice_height));
		
		if (aa > 0 && ((aa-1) < m_heros_to_build))
		{
			Mesh curchar;
			curchar.LoadMesh(GetMeshFilename(m_char_build_tiles[aa-1].GetTileType()));
			curchar.UseTexture(GetTextureFilename(m_char_build_tiles[aa-1].GetTileType()));

			glm::mat4 *translations = new glm::mat4[1];
			translations[0] =
				glm::translate(glm::mat4(1.0),
					glm::vec3(charoriginx, charoriginy, 0));
			curchar.SetUpInstancing(1, char_scalevec, translations);
			delete[] translations;
			curchar.Render();
		}

		if (aa == m_cur_choice_idx)
		{
			float  arrowposx = xdrawpos + (static_cast<float>(m_drawparty_tile.GetRelativeWidth_01()) * 2.0f) * .05f;
			float arrowposy = charoriginy;
			m_texthandler_ptr->Render("-->", arrowposx, arrowposy, TextAlignType::LEFT);
		}
		
		float  textx = charoriginx + (static_cast<float>(m_drawparty_tile.GetRelativeWidth_01()) * 2.0f) * .1f;
		float texty = charoriginy;

		if (aa == 0)
		{
			m_texthandler_ptr->Render(m_save_name, textx, texty, TextAlignType::LEFT);
		}
		else if( aa - 1 < m_charnames.size())
		{
			m_texthandler_ptr->Render(m_charnames[aa-1], textx, texty, TextAlignType::LEFT);
		}
		else
		{
			m_texthandler_ptr->Render("Done", textx, texty, TextAlignType::LEFT);
		}
		glUseProgram(m_primary_shaderid);
	}


}

void PartyBuildMode::GetPartyCharacterNames(std::vector<std::string> &character_names) const
{
	for (auto curname : m_charnames)
	{
		character_names.push_back(curname);
	 }
}

void PartyBuildMode::GetPartyCharacterTypes(std::vector<TileType> &character_types) const
{
	for (int aa = 0; aa < m_heros_to_build; aa++)
	{
		character_types.push_back(m_char_build_tiles[aa].GetTileType());
	}
}
