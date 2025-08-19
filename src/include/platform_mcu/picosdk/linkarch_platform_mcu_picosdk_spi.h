#ifndef LINKARCH_PLATFORM_MCU_PICOSDK_SPI
#define LINKARCH_PLATFORM_MCU_PICOSDK_SPI




/* Includes */

// Hal includes
#include "linkarch_hal_spi.h"

// SPI includes
#include "hardware/spi.h"
#include "hardware/gpio.h"

// Standard C libraries
#include <stdbool.h>
#include <stdint.h>




/* Defines */

typedef spi_inst_t* spi_instance_t;

#define SPI0 spi0
#define SPI1 spi1

#define SPI_CPOL_0 SPI_CPOL_0
#define SPI_CPOL_1 SPI_CPOL_1
#define SPI_CPHA_0 SPI_CPHA_0
#define SPI_CPHA_1 SPI_CPHA_1
#define SPI_MSB_FIRST SPI_MSB_FIRST
#define SPI_LSB_FIRST SPI_LSB_FIRST
#define GPIO_FUNC_SPI GPIO_FUNC_SPI


#define GPIO_IN GPIO_IN
#define GPIO_OUT GPIO_OUT




/* SPI Functions */

bool linkarch_hal_spi_init(spi_instance_t spiNumber, uint32_t baudRate);
bool linkarch_hal_spi_setFormat(spi_instance_t spiNumber, uint8_t dataBits, uint8_t clockPolarity, uint8_t clockPhase, uint8_t bitOrder);
bool linkarch_hal_spi_setFunction(uint8_t pinNumber, uint8_t function);
uint8_t linkarch_hal_spi_write(spi_instance_t spiNumber, const uint8_t *data, uint8_t length);
uint8_t linkarch_hal_spi_read(spi_instance_t spiNumber, uint8_t *data, uint8_t length);
uint8_t linkarch_hal_spi_writeRead(spi_instance_t spiNumber, const uint8_t *dataOut, uint8_t *dataIn, uint8_t length);



#endif // LINKARCH_PLATFORM_MCU_PICOSDK_SPI