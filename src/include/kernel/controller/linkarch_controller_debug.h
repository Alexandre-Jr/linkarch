#ifndef LINKARCH_CONTROLLER_DEBUG
#define LINKARCH_CONTROLLER_DEBUG




// Include controller Parts
#include "linkarch_controller_type.h"

// Include HAL 
#include "linkarch_hal.h"




/* Debugging functions for controller */

static void linkarch_controller_debug_setDebugMessageMutex();
bool linkarch_controller_debug_takeDebugMessageMutex();
bool linkarch_controller_debug_giveDebugMessageMutex();

// Debugging mode transiction

void linkarch_controller_debug_modeTransitionInfos(const linkarch_controller_modeTransition_t *modeTransition);
void linkarch_controller_debug_modeTransitionRequestMessage(linkarch_modeTransition_id_t modeTransitionID);
void linkarch_controller_debug_modeTransitionMessage(bool statusHandleModeTransition, linkarch_modeTransition_id_t modeTransitionID);


// Debugging controller  

void linkarch_controller_debug_controllerInfos(const linkarch_controller_t *controller);




// Debugging Macros
#if LINKARCH_DEBUG_MODE
#define LINKARCH_DEBUG_MESSAGE(message) \
    linkarch_controller_debug_takeDebugMessageMutex(); \
    linkarch_hal_throwDebugMessage("%s\n", message); \
    linkarch_controller_debug_giveDebugMessageMutex();
#define LINKARCH_DEBUG_FUNCTION(function) \
    linkarch_controller_debug_takeDebugMessageMutex(); \
    function;   \
    linkarch_controller_debug_giveDebugMessageMutex();
#else
#define LINKARCH_DEBUG_MESSAGE(message) \
    do { } while (0)
#define LINKARCH_DEBUG_FUNCTION(function) \
    do { } while (0)
#endif // LINKARCH_DEBUG_MODE




#endif // LINKARCH_CONTROLLER_DEBUG