#ifndef CPU_HEADER_FILE
#define CPU_HEADER_FILE

#include "constants.h"
#include "alien.h"
#include "LPTHREAD/lpthread.h"

void cpu ( BRIDGE **bridge, int cardinal );
void planning (BRIDGE **bridge);
#endif