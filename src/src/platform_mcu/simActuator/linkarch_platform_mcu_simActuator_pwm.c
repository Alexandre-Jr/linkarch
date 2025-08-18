#include "linkarch_hal_gpio.h"
#include "linkarch_platform_mcu_simActuator_clientCommand.h"




/* Hal PWM Functions */

bool linkarch_hal_pwm_initPWM(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent)
{

    linkarch_msgDataSize_t data_size = LINKARCH_UINT8_SIZE + 3 * LINKARCH_FLOAT_SIZE; 

    linkarch_msgDataPart_t pwmInit_data[data_size];

    pwmInit_data[0] = pinNumber;
    pwmInit_data[1] = linkarch_getIntegerPart(clkDiv);
    pwmInit_data[2] = linkarch_getDecimalPart(clkDiv);
    pwmInit_data[3] = linkarch_getIntegerPart(freq);
    pwmInit_data[4] = linkarch_getDecimalPart(freq);
    pwmInit_data[5] = linkarch_getIntegerPart(dutyCycle_percent);
    pwmInit_data[6] = linkarch_getDecimalPart(dutyCycle_percent);

    linkarch_command_t pwmInit_cmd = PUT_COMMAND_INIT(PWM_INIT_CMD_ID, data_size, pwmInit_data);

    if (!linkarch_command_sendPutTypeCommand(&pwmInit_cmd)) return false;

    return true;

}

void linkarch_hal_pwm_reConfig(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent)
{

    linkarch_msgDataSize_t data_size = LINKARCH_UINT8_SIZE + 3 * LINKARCH_FLOAT_SIZE;

    linkarch_msgDataPart_t pwmReconfig_data[data_size];

    pwmReconfig_data[0] = pinNumber;
    pwmReconfig_data[1] = linkarch_getIntegerPart(clkDiv);
    pwmReconfig_data[2] = linkarch_getDecimalPart(clkDiv);
    pwmReconfig_data[3] = linkarch_getIntegerPart(freq);
    pwmReconfig_data[4] = linkarch_getDecimalPart(freq);
    pwmReconfig_data[5] = linkarch_getIntegerPart(dutyCycle_percent);
    pwmReconfig_data[6] = linkarch_getDecimalPart(dutyCycle_percent);

    linkarch_command_t pwmReconfig_cmd = PUT_COMMAND_INIT(PWM_RECONFIG_CMD_ID, data_size, pwmReconfig_data);

    if (!linkarch_command_sendPutTypeCommand(&pwmReconfig_cmd)) return;

}

void linkarch_hal_pwm_setDutyCycle(uint8_t pinNumber, float clkDiv, float freq, float dutyCycle_percent)
{

    linkarch_msgDataSize_t data_size = LINKARCH_UINT8_SIZE + 3 * LINKARCH_FLOAT_SIZE;

    linkarch_msgDataPart_t pwmSet_data[data_size];

    pwmSet_data[0] = pinNumber;
    pwmSet_data[1] = linkarch_getIntegerPart(clkDiv);
    pwmSet_data[2] = linkarch_getDecimalPart(clkDiv);
    pwmSet_data[3] = linkarch_getIntegerPart(freq);
    pwmSet_data[4] = linkarch_getDecimalPart(freq);
    pwmSet_data[5] = linkarch_getIntegerPart(dutyCycle_percent);
    pwmSet_data[6] = linkarch_getDecimalPart(dutyCycle_percent);

    linkarch_command_t pwmSet_cmd = PUT_COMMAND_INIT(PWM_SET_DUTY_CYCLE_CMD_ID, data_size, pwmSet_data);

    if (!linkarch_command_sendPutTypeCommand(&pwmSet_cmd)) return;

}

void linkarch_hal_pwm_enablePwm(uint8_t pinNumber, bool enable)
{

    linkarch_msgDataSize_t data_size = LINKARCH_UINT8_SIZE + LINKARCH_BOOL_SIZE;

    linkarch_msgDataPart_t pwmEnable_data[data_size];

    pwmEnable_data[0] = pinNumber;
    pwmEnable_data[1] = enable;

    linkarch_command_t pwmEnable_cmd = PUT_COMMAND_INIT(PWM_ENABLE_CMD_ID, data_size, pwmEnable_data);

    if (!linkarch_command_sendPutTypeCommand(&pwmEnable_cmd)) return;

}