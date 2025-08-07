#ifndef LINKARCH_FSM_EVENT
#define LINKARCH_FSM_EVENT




// FSM state header file
#include "linkarch_fsm_config.h"
#include "linkarch_fsm_state.h"

// Standard c includes
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>




// Define FSM event type
#define INVALID_EVENT NULL 
#define INVALID_TRASITIONACTION NULL



// FSM state type

typedef uint8_t fsm_event_id_t;
typedef bool (*transitionAction_t)(void);

typedef struct
{
    
    fsm_event_id_t fsm_event_id;
    char fsm_event_name[LINKARCH_FSM_MAX_EVENT_NAME_LENGTH];

    fsm_state_id_t sourceStateId;
    fsm_state_id_t targetStateId;

    transitionAction_t beforeTransitionAction;
    transitionAction_t afterTransitionAction;

} fsm_event_t;

// Macros for FSM event management
#define FSM_EVENT_INIT(id, name, sourceState, targetState, beforeAction, afterAction) \
    { .fsm_event_id = id, .fsm_event_name = name, .sourceStateId = sourceState, .targetStateId = targetState, \
      .beforeTransitionAction = beforeAction, .afterTransitionAction = afterAction }




#endif  // LINKARCH_FSM_EVENT
