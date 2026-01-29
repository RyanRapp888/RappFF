#ifndef _GAME_FILE_UTIL_H__
#define _GAME_FILE_UTIL_H__

#include <string>

class GameFileUtil
{
    public:

	static bool LoadGame(const std::string &filename);
	static bool SaveGame(const std::string &filename);
	static void GetListOfSaves();

    private:

    
};


#endif