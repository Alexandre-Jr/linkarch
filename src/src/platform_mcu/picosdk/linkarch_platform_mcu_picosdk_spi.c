#include "linkarch_platform_mcu_picosdk_spi.h"
#include "hardware/spi.h"




/* SPI Functions */

bool linkarch_hal_spi_init(spi_instance_t spiNumber, uint32_t baudRate)
{

    return spi_init(spiNumber, baudRate);

}

bool linkarch_hal_spi_setFormat(spi_instance_t spiNumber, uint8_t dataBits, uint8_t clockPolarity, uint8_t clockPhase, uint8_t bitOrder)
{

    spi_set_format(spiNumber, dataBits, clockPolarity, clockPhase, bitOrder);

    return true;

}

bool linkarch_hal_spi_setFunction(uint8_t pinNumber, uint8_t function)
{

    gpio_set_function(pinNumber, function);

    return true;

}

uint8_t linkarch_hal_spi_write(spi_instance_t spiNumber, const uint8_t *data, uint8_t length)
{   

    return spi_write_blocking(spiNumber, data, length);

}

uint8_t linkarch_hal_spi_read(spi_instance_t spiNumber, uint8_t *data, uint8_t length)
{

    return spi_read_blocking(spiNumber, 0, data, length);

}

uint8_t linkarch_hal_spi_writeRead(spi_instance_t spiNumber, const uint8_t *dataOut, uint8_t *dataIn, uint8_t length)
{

    return spi_write_read_blocking(spiNumber, dataOut, dataIn, length);

}