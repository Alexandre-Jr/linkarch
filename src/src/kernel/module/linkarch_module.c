#include "linkarch_module.h"




/* Debugging functions for module */

void linkarch_module_debug_moduleInfos(const linkarch_module_t *module)
{

    if (module == INVALID_MODULE) {
        linkarch_hal_throwDebugMessage("Module: NULL\n");
        return;
    }

    linkarch_hal_throwDebugMessage("Module ID: %d\n", module->module_id);
    linkarch_hal_throwDebugMessage("Module Name: %s\n", module->module_name);
    linkarch_hal_throwDebugMessage("Module Type: %d\n", module->moduleType);
    linkarch_hal_throwDebugMessage("Module Period in MS: %d\n", module->modulePeriodInMS);

    if (module->startModuleConfig != NULL) {
        linkarch_hal_throwDebugMessage("Module Start Config: Defined\n");
    } else {
        linkarch_hal_throwDebugMessage("Module Start Config: Not Defined\n");
    }

}