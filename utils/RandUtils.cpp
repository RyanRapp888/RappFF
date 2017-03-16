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