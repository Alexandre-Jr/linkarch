#include "linkarch_hal_gpio.h"
#include "linkarch_platform_mcu_simActuator_clientCommand.h"




/* Hal PWM Functions */

bool linkarch_hal_pwm_initPWM(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent)
{

    linkarch_msgDataPart_t pwmInit_data[4] = {pinNumber, clkDiv, freq, dutyCycle_percent};
    linkarch_command_t pwmInit_cmd = PUT_COMMAND_INIT(PWM_INIT_CMD_ID, 4, pwmInit_data);

    if (!linkarch_command_sendPutTypeCommand(&pwmInit_cmd)) return false;

    return true;

}

void linkarch_hal_pwm_reConfig(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent)
{

    linkarch_msgDataPart_t pwmReconfig_data[4] = {pinNumber, clkDiv, freq, dutyCycle_percent};
    linkarch_command_t pwmReconfig_cmd = PUT_COMMAND_INIT(PWM_RECONFIG_CMD_ID, 4, pwmReconfig_data);

    if (!linkarch_command_sendPutTypeCommand(&pwmReconfig_cmd)) return;

}

void linkarch_hal_pwm_setDutyCycle(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent)
{

    linkarch_msgDataPart_t pwmSet_data[4] = {pinNumber, clkDiv, freq, dutyCycle_percent};
    linkarch_command_t pwmSet_cmd = PUT_COMMAND_INIT(PWM_SET_DUTY_CYCLE_CMD_ID, 4, pwmSet_data);

    if (!linkarch_command_sendPutTypeCommand(&pwmSet_cmd)) return;

}

void linkarch_hal_pwm_enablePwm(uint8_t pinNumber, bool enable)
{

    linkarch_msgDataPart_t pwmEnable_data[2] = {pinNumber, enable};
    linkarch_command_t pwmEnable_cmd = PUT_COMMAND_INIT(PWM_ENABLE_CMD_ID, 2, pwmEnable_data);

    if (!linkarch_command_sendPutTypeCommand(&pwmEnable_cmd)) return;

}