#include "linkarch_hal_gpio.h"
#include "linkarch_hal_debug.h"




/* Hal PWM Functions */

bool linkarch_hal_pwm_initPWM(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent)
{

    linkarch_hal_throwDebugMessage("Initializing PWM on pin %d with clkDiv %.2f, freq %.2f Hz, and duty cycle %.2f%%.\n",
                                   pinNumber, clkDiv, freq, dutyCycle_percent);

}

void linkarch_hal_pwm_reConfig(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent)
{

    linkarch_hal_throwDebugMessage("Reconfiguring PWM on pin %d to clkDiv %.2f, freq %.2f Hz, and duty cycle %.2f%%.\n",
                                   pinNumber, clkDiv, freq, dutyCycle_percent);

}

void linkarch_hal_pwm_setDutyCycle(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent)
{

    linkarch_hal_throwDebugMessage("Setting PWM on pin %d to clkDiv %.2f, freq %.2f Hz, and duty cycle %.2f%%.\n",
                                   pinNumber, clkDiv, freq, dutyCycle_percent);

}

void linkarch_hal_pwm_enablePwm(uint8_t pinNumber, bool enable)
{

    linkarch_hal_throwDebugMessage("PWM on pin %d is now %s.\n", pinNumber, enable ? "enabled" : "disabled");

}