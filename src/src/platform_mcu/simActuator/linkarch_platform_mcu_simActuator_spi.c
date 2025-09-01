#include "linkarch_platform_mcu_simActuator_spi.h"
#include "linkarch_platform_mcu_simActuator_clientCommand.h"



/* SPI Functions */

bool linkarch_hal_spi_init(spi_instance_t spiNumber, uint32_t baudRate)
{

    linkarch_msgDataSize_t data_size = LINKARCH_UINT8_SIZE + LINKARCH_UINT32_SIZE;
    
    linkarch_msgDataPart_t spiInit_data[data_size];
    
    spiInit_data[0] = (uint8_t)spiNumber;
    spiInit_data[1] = (uint8_t)(baudRate & 0xFF);
    spiInit_data[2] = (uint8_t)((baudRate >> 8) & 0xFF);
    spiInit_data[3] = (uint8_t)((baudRate >> 16) & 0xFF);
    spiInit_data[4] = (uint8_t)((baudRate >> 24) & 0xFF);

    linkarch_command_t spiInit_cmd = PUT_COMMAND_INIT(SPI_INIT_CMD_ID, data_size, spiInit_data);

    if (!linkarch_command_sendPutTypeCommand(&spiInit_cmd)) return false;

    return true;

}

bool linkarch_hal_spi_setFormat(spi_instance_t spiNumber, uint8_t dataBits, uint8_t clockPolarity, uint8_t clockPhase, uint8_t bitOrder)
{

    linkarch_msgDataSize_t data_size = LINKARCH_UINT8_SIZE + 4 * LINKARCH_UINT8_SIZE;

    linkarch_msgDataPart_t spiSetFormat_data[data_size];

    spiSetFormat_data[0] = (uint8_t)spiNumber;
    spiSetFormat_data[1] = dataBits;
    spiSetFormat_data[2] = clockPolarity;
    spiSetFormat_data[3] = clockPhase;
    spiSetFormat_data[4] = bitOrder;

    linkarch_command_t spiSetFormat_cmd = PUT_COMMAND_INIT(SPI_SET_FORMAT_CMD_ID, data_size, spiSetFormat_data);

    if (!linkarch_command_sendPutTypeCommand(&spiSetFormat_cmd)) return false;

    return true;

}

bool linkarch_hal_spi_setFunction(uint8_t pinNumber, uint8_t function)
{

    linkarch_msgDataSize_t data_size = LINKARCH_UINT8_SIZE + LINKARCH_UINT8_SIZE;

    linkarch_msgDataPart_t spiSetFunction_data[data_size];

    spiSetFunction_data[0] = pinNumber;
    spiSetFunction_data[1] = function;

    linkarch_command_t spiSetFunction_cmd = PUT_COMMAND_INIT(SPI_SET_FUNCTION_CMD_ID, data_size, spiSetFunction_data);

    if (!linkarch_command_sendPutTypeCommand(&spiSetFunction_cmd)) return false;

    return true;

}   

uint8_t linkarch_hal_spi_write(spi_instance_t spiNumber, const uint8_t *data, uint8_t length)
{

    linkarch_msgDataSize_t data_size = 2*LINKARCH_UINT8_SIZE + length * LINKARCH_UINT8_SIZE;

    linkarch_msgDataPart_t spiWrite_data[data_size];

    spiWrite_data[0] = (uint8_t)spiNumber;
    spiWrite_data[1] = length;

    for (uint8_t i = 0; i < length; i++)
    {
        spiWrite_data[i + 2] = data[i];
    }

    linkarch_command_t spiWrite_cmd = PUT_COMMAND_INIT(SPI_WRITE_CMD_ID, data_size, spiWrite_data);

    if (!linkarch_command_sendPutTypeCommand(&spiWrite_cmd)) return 0;

    return length;

}

uint8_t linkarch_hal_spi_read(spi_instance_t spiNumber, uint8_t *data, uint8_t length)
{

    linkarch_msgDataSize_t data_size = LINKARCH_UINT8_SIZE + length;

    linkarch_msgDataPart_t spiRead_data[data_size];

    spiRead_data[0] = (uint8_t)spiNumber;
    for (uint8_t i = 0; i < length; i++)
    {
        spiRead_data[i + 1] = 0;
    }

    linkarch_command_t spiRead_cmd = PUT_COMMAND_INIT(SPI_READ_CMD_ID, data_size, spiRead_data);

    if (!linkarch_command_sendGetTypeCommand(&spiRead_cmd, &data, &length)) return 0;

    return length;

}   

uint8_t linkarch_hal_spi_writeRead(spi_instance_t spiNumber, const uint8_t *dataOut, uint8_t *dataIn, uint8_t length)
{

    linkarch_msgDataSize_t data_size = LINKARCH_UINT8_SIZE + LINKARCH_UINT8_SIZE + length * LINKARCH_UINT8_SIZE;

    linkarch_msgDataPart_t spiWriteRead_data[data_size];

    spiWriteRead_data[0] = (uint8_t)spiNumber;
    spiWriteRead_data[1] = length;

    for (uint8_t i = 0; i < length; i++)
    {
        spiWriteRead_data[i + 2] = dataOut[i];
    }

    linkarch_command_t spiWriteRead_cmd = GET_COMMAND_INIT(SPI_WRITE_READ_CMD_ID, data_size, spiWriteRead_data);

    linkarch_msgData_t temp_dataIn;
    linkarch_msgDataSize_t temp_dataInSize = 0;

    if (!linkarch_command_sendGetTypeCommand(&spiWriteRead_cmd, &temp_dataIn, &temp_dataInSize)) return 0;
    if (temp_dataInSize > length) 
    {
        free(temp_dataIn);
        return 0; 
    }


    for(uint8_t i = 0; i < temp_dataInSize; i++)
    {
        dataIn[length - temp_dataInSize + i] = temp_dataIn[i];
    }

    free(temp_dataIn);

    return length;

}