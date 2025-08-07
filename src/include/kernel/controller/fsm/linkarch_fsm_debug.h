#ifndef LINKARCH_FSM_DEBUG
#define LINKARCH_FSM_DEBUG




// FSM state header file
#include "linkarch_fsm_config.h"
#include "linkarch_fsm_type.h"

// HAL header file
#include "linkarch_hal.h"

// Standard c includes
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>




/* Debugging functions for FSM */

// Debugging FSM state
void linkarch_fsm_debug_stateInfos(const fsm_state_t *state);


// Debugging FSM event
void linkarch_fsm_debug_eventInfos(const fsm_event_t *event);


// Debugging FSM
void linkarch_fsm_debug_fsmInfos(const fsm_t *fsm);




#endif  // LINKARCH_FSM_DEBUG