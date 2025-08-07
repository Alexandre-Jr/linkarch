#include "linkarch_fsm.h"
#include "linkarch_fsm_private.h"



/* FSM public functions */

fsm_state_id_t linkarch_fsm_getActualStateID(const fsm_t *fsm)
{

    if (linkarch_fsm_isThereFsm(fsm)) return INVALID_STATE_ID;

    return fsm->actualStateID;

}

fsm_event_t *linkarch_fsm_getEventByID(const fsm_t *fsm, fsm_event_id_t eventID)
{
    
    if (linkarch_fsm_isThereFsm(fsm)) return INVALID_EVENT;

    if(linkarch_fsm_isValidEventInTheFsm(fsm, eventID)) return INVALID_EVENT;

    return &fsm->listOfEvents[eventID];
    
}

bool linkarch_fsm_handleEvent(fsm_t *fsm, fsm_event_id_t eventID)
{

    fsm_event_t *event = linkarch_fsm_getEventByID(fsm, eventID);

    if (linkarch_fsm_isValidEventToHandle(fsm, event)) return false;

    linkarch_fsm_executeTransitionAction(event->beforeTransitionAction);

    linkarch_fsm_setActualState(fsm, event->targetStateId);

    linkarch_fsm_executeTransitionAction(event->afterTransitionAction);

    return true;

}

bool linkarch_fsm_isTheStateChanged(const fsm_t *fsm, fsm_state_id_t localActualstateID)
{

    if (linkarch_fsm_isThereFsm(fsm)) return false;

    if (linkarch_fsm_isValidStateInTheFsm(fsm, localActualstateID)) return true;

    return (fsm->actualStateID != localActualstateID);

}

/* FSM private functions */

static bool linkarch_fsm_isThereFsm(const fsm_t *fsm)
{

    return (fsm == INVALID_FSM);

}

static bool linkarch_fsm_setActualState(fsm_t *fsm, fsm_state_id_t newStateID)
{

    if (linkarch_fsm_isValidStateInTheFsm(fsm, newStateID)) return false;

    fsm->actualStateID = newStateID;
    return true;

}

static void linkarch_fsm_executeTransitionAction(const transitionAction_t action)
{

    if (linkarch_fsm_isValidTranstionAction(action)) return;

    action();

}


static bool linkarch_fsm_isValidTranstionAction(const transitionAction_t action)
{

    return (action == INVALID_TRASITIONACTION);

}

static bool linkarch_fsm_isValidStateInTheFsm(const fsm_t *fsm, fsm_state_id_t stateID)
{

    return (linkarch_fsm_isThereFsm(fsm) || stateID >= fsm->numberOfStates);

}

static bool linkarch_fsm_isValidEventInTheFsm(const fsm_t *fsm, fsm_event_id_t eventID)
{

    return (linkarch_fsm_isThereFsm(fsm) || eventID >= fsm->numberOfEvents);

}

static bool linkarch_fsm_isValidEventToHandle(const fsm_t *fsm, fsm_event_t * event)
{

    return (linkarch_fsm_isThereFsm(fsm) || event == INVALID_EVENT || event->fsm_event_id >= fsm->numberOfEvents || event->sourceStateId != fsm->actualStateID);

}



/* Debugging functions for FSM */

void linkarch_fsm_debug_stateInfos(const fsm_state_t *state)
{

    if (state == INVALID_STATE) {
        linkarch_hal_throwDebugMessage("FSM State: NULL\n");
        return;
    }

    linkarch_hal_throwDebugMessage("FSM State ID: %d\n", state->fsm_state_id);
    linkarch_hal_throwDebugMessage("FSM State Name: %s\n", state->fsm_state_name);

}

void linkarch_fsm_debug_eventInfos(const fsm_event_t *event)
{

    if (event == INVALID_EVENT) {
        linkarch_hal_throwDebugMessage("FSM Event: NULL\n");
        return;
    }

    linkarch_hal_throwDebugMessage("FSM Event ID: %d\n", event->fsm_event_id);
    linkarch_hal_throwDebugMessage("FSM Event Name: %s\n", event->fsm_event_name);
    linkarch_hal_throwDebugMessage("FSM Event Source State ID: %d\n", event->sourceStateId);
    linkarch_hal_throwDebugMessage("FSM Event Target State ID: %d\n", event->targetStateId);

    if (event->beforeTransitionAction != NULL) {
        linkarch_hal_throwDebugMessage("FSM Event Before Transition Action: Defined\n");
    } else {
        linkarch_hal_throwDebugMessage("FSM Event Before Transition Action: Not Defined\n");
    }

    if (event->afterTransitionAction != NULL) {
        linkarch_hal_throwDebugMessage("FSM Event After Transition Action: Defined\n");
    } else {
        linkarch_hal_throwDebugMessage("FSM Event After Transition Action: Not Defined\n");
    }

}

void linkarch_fsm_debug_fsmInfos(const fsm_t *fsm)
{

    if (linkarch_fsm_isThereFsm(fsm)) {
        linkarch_hal_throwDebugMessage("FSM: NULL\n");
        return;
    }

    linkarch_hal_throwDebugMessage("FSM Current State ID: %d\n", fsm->actualStateID);
    linkarch_hal_throwDebugMessage("FSM Current State Name: %s\n", 
                                   fsm->listOfStates[fsm->actualStateID].fsm_state_name);

    linkarch_hal_throwDebugMessage("FSM States:\n");
    for (int i = 0; i < fsm->numberOfStates; i++) {
        linkarch_fsm_debug_stateInfos(&fsm->listOfStates[i]);
    }
    
    linkarch_hal_throwDebugMessage("FSM Events:\n");
    for (int i = 0; i < fsm->numberOfEvents; i++) {
        linkarch_fsm_debug_eventInfos(&fsm->listOfEvents[i]);
    }

}