#ifndef LINKARCH_FSM
#define LINKARCH_FSM




// Include FSM Parts
#include "linkarch_fsm_type.h"
#include "linkarch_fsm_debug.h"




/* FSM public functions */


// Get functions

fsm_state_id_t linkarch_fsm_getActualStateID(const fsm_t *fsm);
fsm_event_t *linkarch_fsm_getEventByID(const fsm_t *fsm, fsm_event_id_t eventID);


// User functions

bool linkarch_fsm_handleEvent(fsm_t *fsm, fsm_event_id_t eventID);
bool linkarch_fsm_isTheStateChanged(const fsm_t *fsm, fsm_state_id_t localActualstateID);




#endif  // LINKARCH_FSM
