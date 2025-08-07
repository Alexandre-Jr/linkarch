#ifndef LINKARCH_MODULE_TYPE
#define LINKARCH_MODULE_TYPE




// Import linkarch configuration
#include "linkarch_config.h"

// Include osal header
#include "linkarch_osal.h"

// Include standard C headers
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>




// Defines
#define INVALID_MODULE NULL




// linkarch module type

typedef uint8_t linkarch_module_id_t;

typedef bool (*startModuleConfig_t)(void);

typedef enum
{

    LINKARCH_MODULETYPE_LOOP_CLOCKED = 0,
    LINKARCH_MODULETYPE_LOOP_UNCLOCKED,
    NUMBER_OF_MODULETYPE

} linkarch_module_moduleTypes_t;

typedef uint8_t linkarch_moduleType_t;

typedef uint16_t linkarch_modulePeriodInMS_t;

typedef struct
{

    // Public variables
    linkarch_module_id_t module_id;
    const char module_name[LINKARCH_MODULE_MAX_NAME_LENGTH];  
    startModuleConfig_t startModuleConfig;
    linkarch_moduleType_t moduleType;
    linkarch_modulePeriodInMS_t modulePeriodInMS;
    uint16_t moduleStackSize;
    uint16_t modulePriority;
    bool isCoreAffinity;
    uint8_t coreNumber;

    // Sencitive variables
    linkarch_taskhandle_t moduleTaskHandle;
    linkarch_semaphorehandle_t loopModuleSemaphore;
    linkarch_tick_t lastWakeTime; 
    
} linkarch_module_t;




// Macros for linkarch module management

#define LINKARCH_MODULE_INIT(id, name, startConfig, type, period, stackSize, priority, isCA, core) \
    { .module_id = id, .module_name = name, .startModuleConfig = startConfig, .moduleType = type, .modulePeriodInMS = period, .moduleStackSize = stackSize, .modulePriority = priority, \
        .isCoreAffinity = isCA, .coreNumber = core, \
      .moduleTaskHandle = LINKARCH_INVALID_TASK, .loopModuleSemaphore = NULL, .lastWakeTime = period}




#endif  // LINKARCH_MODULE_TYPE
