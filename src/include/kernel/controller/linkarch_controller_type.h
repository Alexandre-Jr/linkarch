#ifndef LINKARCH_CONTROLLER_TYPE
#define LINKARCH_CONTROLLER_TYPE




// Include controller Parts
#include "linkarch_controller_modeTransition.h"

// Include linkarch structural types
#include "linkarch_module.h"
#include "linkarch_mode.h"

// Include FSM Parts
#include "linkarch_fsm.h"

// Include linkarch OSAL
#include "linkarch_osal.h"




// Defines
#define INVALID_CONTROLLER NULL




// Controller type

typedef struct
{

    linkarch_module_t * listOfModules;

    linkarch_mode_t * listOfModes;

    linkarch_controller_modeTransition_t * listOfModeTransitions;

    fsm_t * linkarch_controller_fsm;

    linkarch_queuehandle_t modeTransition_queue;

    linkarch_taskhandle_t controllerTaskHandle;

} linkarch_controller_t;




// Macros for controller management
#define LINKARCH_CONTROLLER_INIT(modules, modes, modeTransitions) \
    { .listOfModules = modules, .listOfModes = modes, .listOfModeTransitions = modeTransitions, .linkarch_controller_fsm = NULL, .modeTransition_queue = NULL }




#endif  // LINKARCH_CONTROLLER_TYPE
