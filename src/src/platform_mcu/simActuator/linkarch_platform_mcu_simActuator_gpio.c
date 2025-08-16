#include "linkarch_hal_gpio.h"
#include "linkarch_platform_mcu_simActuator_clientCommand.h"


#include <string.h>

// Hal gpio functions

bool linkarch_hal_gpio_init(uint8_t pinNumber, uint8_t pinMode)
{
    
    linkarch_msgDataPart_t gpioInit_data[2] = {pinNumber, pinMode};
    linkarch_command_t gpioInit_cmd = PUT_COMMAND_INIT(GPIO_INIT_CMD_ID, 2, gpioInit_data);

    if (!linkarch_command_sendPutTypeCommand(&gpioInit_cmd)) return false;

    return true;

}

bool linkarch_hal_gpio_read(uint8_t pinNumber)
{

    linkarch_msgDataPart_t gpioGet_data[1] = {pinNumber};
    linkarch_command_t gpioGet_cmd = GET_COMMAND_INIT(GPIO_GET_CMD_ID, 1, gpioGet_data);

    linkarch_msgData_t temp_receivedData = NULL;
    linkarch_msgDataSize_t receivedDataSize = 0;

    if (!linkarch_command_sendGetTypeCommand(&gpioGet_cmd, &temp_receivedData, &receivedDataSize)) return false;

    if (receivedDataSize != 1 && temp_receivedData != NULL) {
        free(temp_receivedData);
        return false;
    }

    bool value = temp_receivedData[0];
    free(temp_receivedData);
    
    return value;

}

bool linkarch_hal_gpio_put(uint8_t pinNumber, bool value)
{

    printf("[linkarch hal gpio]: Setting pin %d to %d\n", pinNumber, value);
    
    linkarch_msgDataPart_t gpioSet_data[2] = {pinNumber, value};
    linkarch_command_t gpioSet_cmd = PUT_COMMAND_INIT(GPIO_SET_CMD_ID, 2, gpioSet_data);

    if(!linkarch_command_sendPutTypeCommand(&gpioSet_cmd)) return false;
    
    return true;

}