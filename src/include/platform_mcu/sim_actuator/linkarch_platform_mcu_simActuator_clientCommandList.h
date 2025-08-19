#ifndef LINKARCH_PLATFORM_MCU_SA_CLIENTCOMMANDLIST
#define LINKARCH_PLATFORM_MCU_SA_CLIENTCOMMANDLIST


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
    ADC_INIT_CMD_ID,
    ADC_INIT_PIN_CMD_ID,
    ADC_READ_CMD_ID,
    PWM_INIT_CMD_ID,
    PWM_RECONFIG_CMD_ID,
    PWM_SET_DUTY_CYCLE_CMD_ID,
    PWM_ENABLE_CMD_ID,
    SPI_INIT_CMD_ID,
    SPI_SET_FORMAT_CMD_ID,
    SPI_SET_FUNCTION_CMD_ID,
    SPI_WRITE_CMD_ID,
    SPI_READ_CMD_ID,
    SPI_WRITE_READ_CMD_ID,
    NUMBER_OF_CMDS

} CommandIDList;

typedef uint8_t linkarch_command_id_t;




#endif  // LINKARCH_PLATFORM_MCU_SA_CLIENTCOMMANDLIST