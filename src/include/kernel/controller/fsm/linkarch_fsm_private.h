#ifndef LINKARCH_FSM_PRIVATE
#define LINKARCH_FSM_PRIVATE




// Include FSM Parts
#include "linkarch_fsm_config.h"
#include "linkarch_fsm_type.h"




/* FSM private functions */


// Action functions

static bool linkarch_fsm_setActualState(fsm_t *fsm, fsm_state_id_t newStateID);
static void linkarch_fsm_executeTransitionAction(const transitionAction_t action);


// Validation functions

inline static bool linkarch_fsm_isValidTranstionAction(const transitionAction_t action);
inline static bool linkarch_fsm_isValidStateInTheFsm(const fsm_t *fsm, fsm_state_id_t stateID);
inline static bool linkarch_fsm_isValidEventInTheFsm(const fsm_t *fsm, fsm_event_id_t eventID);
inline static bool linkarch_fsm_isValidEventToHandle(const fsm_t *fsm, fsm_event_t * event);
inline static bool linkarch_fsm_isThereFsm(const fsm_t *fsm);




#endif  // LINKARCH_FSM_PRIVATE
