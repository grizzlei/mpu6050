#include "utils.h"


////////////////////////////////////////////


double get_time_sec()
{
	struct timespec now = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (double)now.tv_sec + 1e-9 * now.tv_nsec;
}


////////////////////////////////////////////


int sysguard(int res, char *msg)
{
	if (res == -1)
	{
		prerr("%s (%s)", msg, strerror(errno));
	}
	return res;
}


////////////////////////////////////////////