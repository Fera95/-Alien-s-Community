#ifndef BRIDGE_HEADER_FILE
#define BRIDGE_HEADER_FILE

#include "constants.h"

void create_bridge( BRIDGE **ctxBridge, int length, int weight, enum bridgePosition position, int scheduler);
BRIDGE * load_bridge(enum bridgePosition pos);

#endif

