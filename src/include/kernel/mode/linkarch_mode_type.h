#ifndef LINKARCH_MODE_TYPE
#define LINKARCH_MODE_TYPE




// Import linkarch configuration
#include "linkarch_config.h"

// Include standard C headers
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>




// Defines
#define INVALID_MODE NULL
#define INVALID_MODE_ID 0xFF




// linkarch module type

typedef uint8_t linkarch_mode_id_t;

typedef void (*modeFunctions)(void);

typedef struct
{

    modeFunctions setup_function;
    modeFunctions loop_function;
    modeFunctions shutdown_function;

} linkarch_modeConfigToModule_t;

typedef struct
{

    linkarch_mode_id_t mode_id;
    char mode_name[LINKARCH_MODE_MAX_NAME_LENGTH];
    linkarch_modeConfigToModule_t * listOfModeConfigToModule;

} linkarch_mode_t;




// Macros for linkarch module management

#define LINKARCH_MODECONFIGTTOMODULE_INIT(setupFunc, loopFunc, shutdownFunc) \
    { .setup_function = setupFunc, .loop_function = loopFunc, .shutdown_function = shutdownFunc }

#define LINKARCH_MODE_INIT(id, name, listOfModeConfigToMod) \
    { .mode_id = id, .mode_name = name, .listOfModeConfigToModule = listOfModeConfigToMod }




#endif  // LINKARCH_MODE_TYPE
