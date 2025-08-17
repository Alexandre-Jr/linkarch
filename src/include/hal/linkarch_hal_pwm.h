#ifndef LINKARCH_HAL_PWM
#define LINKARCH_HAL_PWM




/* Includes */

// Standard C libraries
#include <stdbool.h>
#include <stdint.h>




/* Hal PWM Functions */

bool linkarch_hal_pwm_initPWM(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent);
void linkarch_hal_pwm_reConfig(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent);
void linkarch_hal_pwm_setDutyCycle(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent);
void linkarch_hal_pwm_enablePwm(uint8_t pinNumber, bool enable);




#endif  // LINKARCH_HAL_PWM