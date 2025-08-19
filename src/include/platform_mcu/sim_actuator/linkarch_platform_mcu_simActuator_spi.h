#ifndef LINKARCH_PLATFORM_MCU_SIMACTUATOR_SPI
#define LINKARCH_PLATFORM_MCU_SIMACTUATOR_SPI




/* Includes */

// Hal includes
#include "linkarch_hal_spi.h"

// Standard C libraries
#include <stdbool.h>
#include <stdint.h>


/* Defines */

typedef uint8_t spi_instance_t;

#define SPI0 0
#define SPI1 1

#define SPI_CPOL_0 0
#define SPI_CPOL_1 1
#define SPI_CPHA_0 0
#define SPI_CPHA_1 1
#define SPI_MSB_FIRST 0
#define SPI_LSB_FIRST 1
#define GPIO_FUNC_SPI 0

#define GPIO_IN 0
#define GPIO_OUT 1




/* SPI Functions */

bool linkarch_hal_spi_init(spi_instance_t spiNumber, uint32_t baudRate);
bool linkarch_hal_spi_setFormat(spi_instance_t spiNumber, uint8_t dataBits, uint8_t clockPolarity, uint8_t clockPhase, uint8_t bitOrder);
bool linkarch_hal_spi_setFunction(uint8_t pinNumber, uint8_t function);
uint8_t linkarch_hal_spi_write(spi_instance_t spiNumber, const uint8_t *data, uint8_t length);
uint8_t linkarch_hal_spi_read(spi_instance_t spiNumber, uint8_t *data, uint8_t length);
uint8_t linkarch_hal_spi_writeRead(spi_instance_t spiNumber, const uint8_t *dataOut, uint8_t *dataIn, uint8_t length);




#endif // LINKARCH_PLATFORM_MCU_SIMACTUATOR_SPI