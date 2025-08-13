#ifndef LINKARCH_PLATFORM_MCU_CLIENTCOMMANDLIST
#define LINKARCH_PLATFORM_MCU_CLIENTCOMMANDLIST


/* Includes */

// Standard C Library
#include <stdint.h>




/* Defines */

// Client Command list

typedef enum 
{

    GPIO_INIT_CMD_ID = 0,
    GPIO_SET_CMD_ID,
    GPIO_GET_CMD_ID,
    NUMBER_OF_CMDS

} CommandIDList;

typedef uint8_t linkarch_command_id_t;




#endif  // LINKARCH_PLATFORM_MCU_CLIENTCOMMANDLIST