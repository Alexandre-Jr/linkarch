#include "linkarch_hal_gpio.h"
#include "linkarch_platform_mcu_simActuator_clientCommand.h"




/* GPIO Functions */

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
    
    linkarch_msgDataPart_t gpioSet_data[2] = {pinNumber, value};
    linkarch_command_t gpioSet_cmd = PUT_COMMAND_INIT(GPIO_SET_CMD_ID, 2, gpioSet_data);

    if(!linkarch_command_sendPutTypeCommand(&gpioSet_cmd)) return false;
 
    return true;

}


/* ADC Functions */

bool linkarch_hal_gpio_adc_init(void)
{

    linkarch_command_t adcInit_cmd = PUT_COMMAND_INIT(ADC_INIT_CMD_ID, 0, NULL);

    if (!linkarch_command_sendPutTypeCommand(&adcInit_cmd)) return false;

    return true;

}

bool linkarch_hal_gpio_adc_initPin(uint8_t pinNumber)
{

    linkarch_msgDataPart_t adcInitPin_data[1] = {pinNumber};
    linkarch_command_t adcInitPin_cmd = PUT_COMMAND_INIT(ADC_INIT_PIN_CMD_ID, 1, adcInitPin_data);

    if (!linkarch_command_sendPutTypeCommand(&adcInitPin_cmd)) return false;

    return true;

}

float linkarch_hal_gpio_adc_read(uint8_t pinNumber)
{

    linkarch_msgDataPart_t adcRead_data[1] = {pinNumber};
    linkarch_command_t adcRead_cmd = GET_COMMAND_INIT(ADC_READ_CMD_ID, 1, adcRead_data);

    linkarch_msgData_t temp_receivedData = NULL;
    linkarch_msgDataSize_t receivedDataSize = 0;

    if (!linkarch_command_sendGetTypeCommand(&adcRead_cmd, &temp_receivedData, &receivedDataSize)) return -1.0f;

    if (receivedDataSize != 2 && temp_receivedData != NULL) {
        free(temp_receivedData);
        return -1.0f;
    }

    float value = linkarch_getFloatFromParts(temp_receivedData[0], temp_receivedData[1]);
    free(temp_receivedData);
    
    return value;

}