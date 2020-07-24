#ifndef BRIDGE_HEADER_FILE
#define BRIDGE_HEADER_FILE

#include "constants.h"
#include <stdio.h>
#include "path.h"


#define EAST_BRIDGE_CONF_PATH  "./config/east_bridge.conf"
#define MID_BRIDGE_CONF_PATH  "./config/mid_bridge.conf"
#define WEST_BRIDGE_CONF_PATH  "./config/west_bridge.conf"
// #define AUX_FILE_PATH  "../config/config.aux"

void create_bridge (BRIDGE **ctxBridge, config_bridge bridgeConf,  enum bridgePosition position );
void print_bridge(BRIDGE * bridge2print);
config_bridge load_bridge(enum bridgePosition pos);

#endif

