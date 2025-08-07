#ifndef LINKARCH_CONTROLLER_MODULETRANSITION
#define LINKARCH_CONTROLLER_MODULETRANSITION




// Import linkarch configuration
#include "linkarch_config.h"

// Include linkarch module type
#include "linkarch_mode_type.h"

// Include standard C headers
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>




// Defines
#define INVALID_MODULETRANSITION NULL




// linkarch module type

typedef uint8_t linkarch_modeTransition_id_t;

typedef bool (*transitionFunction_t)(void);

typedef struct linkarch_controller_modeTransition
{
    linkarch_modeTransition_id_t modeTransition_id;
    char modeTransition_name[LINKARCH_MODE_TRANSITION_MAX_NAME_LENGTH];

    linkarch_mode_id_t sourceModeID;
    linkarch_mode_id_t targetModeID;

    transitionFunction_t endTransitionFunction;
    transitionFunction_t startTransitionFunction;

} linkarch_controller_modeTransition_t;




// Macros for linkarch module management

#define LINKARCH_MODETRANSITION_INIT(id, name, sourceMode, targetMode, endTransitionFunc, startTransitionFunc) \
    { .modeTransition_id = id, .modeTransition_name = name, .sourceModeID = sourceMode, .targetModeID = targetMode, \
      .endTransitionFunction = endTransitionFunc, .startTransitionFunction = startTransitionFunc }




#endif  // LINKARCH_CONTROLLER_MODULETRANSITION
