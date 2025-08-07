#ifndef LINKARCH_FSM_STATE
#define LINKARCH_FSM_STATE




// FSM state header file
#include "linkarch_fsm_config.h"

// Standard c includes
#include <stdint.h>
#include <stddef.h>



// Defines
#define INVALID_STATE NULL
#define INVALID_STATE_ID -1 




// FSM state type

typedef uint8_t fsm_state_id_t;

typedef struct
{
    
    fsm_state_id_t fsm_state_id;
    char fsm_state_name[LINKARCH_FSM_MAX_STATE_NAME_LENGTH];

} fsm_state_t;




// Macros for FSM state management
#define FSM_STATE_INIT(id, name) \
    { .fsm_state_id = id, .fsm_state_name = name }




#endif  // LINKARCH_FSM_STATE
