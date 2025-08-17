#ifndef LINKARCH_CONTROLLER_DEBUG
#define LINKARCH_CONTROLLER_DEBUG




// Include controller Parts
#include "linkarch_controller_type.h"

// Include HAL 
#include "linkarch_hal.h"




/* Debugging functions for controller */

void linkarch_controller_debug_setDebugMessageMutex();
bool linkarch_controller_debug_takeDebugMessageMutex();
bool linkarch_controller_debug_giveDebugMessageMutex();

// Debugging mode transiction

void linkarch_controller_debug_modeTransitionInfos(const linkarch_controller_modeTransition_t *modeTransition);
void linkarch_controller_debug_modeTransitionRequestMessage(linkarch_modeTransition_id_t modeTransitionID);
void linkarch_controller_debug_modeTransitionMessage(bool statusHandleModeTransition, linkarch_modeTransition_id_t modeTransitionID);


// Debugging controller  

void linkarch_controller_debug_controllerInfos(const linkarch_controller_t *controller);




#endif // LINKARCH_CONTROLLER_DEBUG