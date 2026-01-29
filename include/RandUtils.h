#ifndef _RAND_UTILS__H
#define _RAND_UTILS__H

#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>
#include <algorithm>
#include <vector>


int get_rand_0_99();
int get_rand_0_x(int x);

bool is_blank(const std::string &str);
std::string to_upper(const std::string &str);

void get_files(const std::string &dir_to_process,
	const std::string &file_ext,
	std::vector<std::string> &files);

#endif