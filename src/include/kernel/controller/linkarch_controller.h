#ifndef LINKARCH_CONTROLLER
#define LINKARCH_CONTROLLER




// Include controller Parts
#include "linkarch_controller_type.h"
#include "linkarch_controller_debug.h"

// Include fsm
#include "linkarch_fsm.h"

// Standard C includes
#include <string.h>




/* Controller public functions */

// User functions

bool linkarch_controller_config(linkarch_controller_t * controller, linkarch_mode_id_t initialModeID);
bool linkarch_controller_startSystem();
bool linkarch_controller_requestHandleModeTransition(linkarch_modeTransition_id_t modeTrasitionID);
bool linkarch_controller_isTheModeChanged(linkarch_mode_id_t localActualModeID);




#endif  // LINKARCH_CONTROLLER