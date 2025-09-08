#include "linkarch_hal_pwm.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"




/* Defines */
#define FREQUENCY_CPU 125000000 // 125 MHz for Raspberry Pi Pico


/* Hal PWM Functions */

bool linkarch_hal_pwm_initPWM(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent)
{

    gpio_set_function(pinNumber, GPIO_FUNC_PWM);

    uint8_t slice_num = pwm_gpio_to_slice_num(pinNumber);
    uint8_t chan_num = pwm_gpio_to_channel(pinNumber);

    pwm_set_clkdiv(slice_num, clkDiv);

    uint32_t f_clock = FREQUENCY_CPU;
    uint32_t wrap = f_clock/(clkDiv * freq) - 1;
    pwm_set_wrap(slice_num, wrap);

    uint32_t chan_level = (dutyCycle_percent/100)*(wrap + 1);
    pwm_set_chan_level(slice_num, chan_num, chan_level);
    pwm_set_enabled(slice_num, true);

    return true;

}

void linkarch_hal_pwm_reConfig(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent)
{

    uint8_t slice_num = pwm_gpio_to_slice_num(pinNumber);
    uint8_t chan_num = pwm_gpio_to_channel(pinNumber);

    pwm_set_clkdiv(slice_num, clkDiv);

    uint32_t f_clock = FREQUENCY_CPU;
    uint32_t wrap = f_clock/(clkDiv * freq) - 1;
    pwm_set_wrap(slice_num, wrap);

    uint32_t chan_level = (dutyCycle_percent/100)*(wrap + 1);
    pwm_set_chan_level(slice_num, chan_num, chan_level);
    pwm_set_enabled(slice_num, true);

}

void linkarch_hal_pwm_setDutyCycle(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent)
{

    uint8_t slice_num = pwm_gpio_to_slice_num(pinNumber);
    uint8_t chan_num = pwm_gpio_to_channel(pinNumber);

    uint32_t f_clock = FREQUENCY_CPU;
    uint32_t wrap = f_clock/(clkDiv * freq) - 1;
    uint32_t chan_level = (uint32_t)((dutyCycle_percent / 100.0f) * (wrap + 1));

    pwm_set_chan_level(slice_num, chan_num, chan_level);

}

void linkarch_hal_pwm_enablePwm(uint8_t pinNumber, bool enable)
{

    uint8_t slice_num = pwm_gpio_to_slice_num(pinNumber);

    pwm_set_enabled(slice_num, true);

}