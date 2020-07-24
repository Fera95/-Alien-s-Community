#ifndef PATH_HEADER_FILE
#define PATH_HEADER_FILE

#include "constants.h"

void create_path(PATH * newPath, float x, float y, int width, int height);
void unlock_path(PATH * nextPath);
int lock_path(PATH * nextPath, int id);
#endif