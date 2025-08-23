#include "linkarch_hal_debug.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"




/* GPIO Functions */

bool linkarch_hal_gpio_init(uint8_t pinNumber, uint8_t pinMode)
{

    gpio_init(pinNumber);
    gpio_set_dir(pinNumber, pinMode);

    return true;

}

bool linkarch_hal_gpio_read(uint8_t pinNumber)
{

    return gpio_get(pinNumber);

}

bool linkarch_hal_gpio_put(uint8_t pinNumber, bool value)
{

    gpio_put(pinNumber, value);
    
    return true;

}


/* ADC Functions */

bool linkarch_hal_gpio_adc_init(void)
{

    adc_init();

    return true;

}

bool linkarch_hal_gpio_adc_initPin(uint8_t pinNumber)
{

    adc_gpio_init(pinNumber);

    return true;

}

uint8_t linkarch_hal_gpio_adc_pinNumberToAdcChannel(uint8_t pinNumber)
{

    return pinNumber - 26;

}

uint16_t linkarch_hal_gpio_adc_readRaw(uint8_t pinNumber)
{

    adc_select_input(linkarch_hal_gpio_adc_pinNumberToAdcChannel(pinNumber));

    return adc_read();

}

float linkarch_hal_gpio_adc_rawReadToFloat(uint16_t adcValue)
{

    return (adcValue * 3.3f) / (1 << 12);

}

float linkarch_hal_gpio_adc_read(uint8_t pinNumber)
{

    uint16_t adcValue = linkarch_hal_gpio_adc_readRaw(pinNumber);

    return linkarch_hal_gpio_adc_rawReadToFloat(adcValue);;

}