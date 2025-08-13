#include "linkarch_hal_gpio.h"
#include "linkarch_hal_debug.h"




// Hal gpio functions

bool linkarch_hal_gpio_init(uint8_t pinNumber)
{

    linkarch_hal_throwDebugMessage("GPIO pin %d initialized successfully.\n", pinNumber);
    return true;

}

bool linkarch_hal_gpio_read(uint8_t pinNumber)
{

    linkarch_hal_throwDebugMessage("GPIO pin %d read successfully.\n", pinNumber);
    return true;

}

bool linkarch_hal_gpio_put(uint8_t pinNumber, bool value)
{

    linkarch_hal_throwDebugMessage("GPIO pin %d set to %s successfully.\n", pinNumber, value ? "HIGH" : "LOW");
    return true;

}