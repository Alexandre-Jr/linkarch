#ifndef LINKARCH_FSM_TYPE
#define LINKARCH_FSM_TYPE




// Include FSM Parts
#include "linkarch_fsm_config.h"
#include "linkarch_fsm_state.h"
#include "linkarch_fsm_event.h"



// Defines
#define INVALID_FSM NULL




// FSM type
typedef struct
{

    fsm_state_t * listOfStates;
    fsm_event_t * listOfEvents;
    fsm_state_id_t actualStateID;

    uint8_t numberOfStates;
    uint8_t numberOfEvents;

} fsm_t;




// Macros for FSM management
#define FSM_INIT(states, events, initialStateID, nstates, nevents) \
    { .listOfStates = states, .listOfEvents = events, .actualStateID = initialStateID, .numberOfStates = nstates, .numberOfEvents = nevents }




#endif  // LINKARCH_FSM_TYPE