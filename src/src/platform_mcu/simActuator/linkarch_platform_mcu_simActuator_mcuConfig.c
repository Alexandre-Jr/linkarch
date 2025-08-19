#include "linkarch_hal_mcuConfig.h"
#include "linkarch_hal_debug.h"

// Sim Actuator Includes
#include "linkarch_platform_mcu_simActuator_clientConnection.h"

// Osal
#include "linkarch_osal.h"



// Hal mcuConfig functions

bool linkarch_hal_mcuInit(void)
{
    
    if(!linkarch_clientConnection_init()) return false;

    linkarch_osal_delayMs(100);

    return true;

}