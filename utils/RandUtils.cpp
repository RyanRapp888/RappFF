#include "RandUtils.h"
#include <cstdlib>

int get_rand_0_99()
{
	return get_rand_0_x(99);
}

int get_rand_0_x(int x)
{
	static bool is_seeded(false);
	if (!is_seeded)
	{
		srand(14);
		is_seeded = true;
	}
	int result = rand() % (x+1);
	return result;
}

bool is_blank(const std::string &str)
{
	return (str.find_first_not_of(' ') == std::string::npos);
}

//file_ext should not include asterisk or dot, just the character extension
void get_files(const std::string &dir_to_process, 
	                 const std::string &file_ext, 
	                 std::vector<std::string> &files)
{
	std::string dir_with_wildcard = dir_to_process;
	dir_with_wildcard += "\\*." + file_ext;
	HANDLE hFind;
	WIN32_FIND_DATA findData;
	
	hFind = FindFirstFile(dir_with_wildcard.c_str(), &findData);
	std::string tmp(findData.cFileName);
	if (!(findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY))
	{
		files.emplace_back(dir_to_process + "\\" + findData.cFileName);
	}

	while (FindNextFile(hFind, &findData))
	{
		std::string tmp(findData.cFileName);
		if (!(findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY))
		{
			files.emplace_back(dir_to_process + "\\" + findData.cFileName);
		}
	}

	FindClose(hFind);
}