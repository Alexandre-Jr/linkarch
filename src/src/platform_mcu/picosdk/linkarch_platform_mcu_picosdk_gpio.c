#include "linkarch_hal_debug.h"
#include "pico/stdlib.h"


// Hal gpio functions

bool linkarch_hal_gpio_init(unit8_t pinNumber, uint8_t pinMode)
{

    gpio_init(pinNumber);
    gpio_set_dir(pinNumber, pinMode);

    return true;

}

bool linkarch_hal_gpio_read(uint8_t pinNumber)
{

    return gpio_get(pinNumber);

    return true;

}

bool linkarch_hal_gpio_put(uint8_t pinNumber, bool value)
{

    gpio_put(pinNumber, value);
    
    return true;

}