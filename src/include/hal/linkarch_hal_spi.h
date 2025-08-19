#ifndef LINKARCH_HAL_SPI
#define LINKARCH_HAL_SPI




/* Includes */

// Types
#if defined(TARGET_MCU_PICOSDK)
    #include "platform_mcu/picosdk/linkarch_platform_mcu_picosdk_spi.h"
#elif defined(TARGET_MCU_PCCONSOLEDEBUG)
    #include "platform_mcu/pc_console_debug/linkarch_platform_mcu_pcConsoleDebug_spi.h"
#elif defined(TARGET_MCU_SIMACTUATOR)
    #include "platform_mcu/sim_actuator/linkarch_platform_mcu_simActuator_spi.h"
#else
    #error "Unsupported MCU platform"
#endif




#endif  // LINKARCH_HAL_SPI