#include "linkarch_hal_mcuConfig.h"
#include "linkarch_hal_debug.h"

// Sim Actuator Includes
#include "linkarch_platform_mcu_simActuator_clientConnection.h"




// Hal mcuConfig functions

bool linkarch_hal_mcuInit(void)
{
    
    return linkarch_clientConnection_init();

}