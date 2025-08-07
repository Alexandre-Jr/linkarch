#include "linkarch_mode.h"




/* Debugging functions for module */

void linkarch_mode_debug_modeInfos(const linkarch_mode_t *module)
{

    if (module == INVALID_MODE) {
        linkarch_hal_throwDebugMessage("Mode: NULL\n");
        return;
    }

    linkarch_hal_throwDebugMessage("Mode ID: %d\n", module->mode_id);
    linkarch_hal_throwDebugMessage("Mode Name: %s\n", module->mode_name);

    if (module->listOfModeConfigToModule != NULL) {
        linkarch_hal_throwDebugMessage("Mode Config to Modules: Defined\n");
    } else {
        linkarch_hal_throwDebugMessage("Mode Config to Modules: Not Defined\n");
    }

}