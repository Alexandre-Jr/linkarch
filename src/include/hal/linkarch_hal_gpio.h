#ifndef LINKARCH_HAL_GPIO
#define LINKARCH_HAL_GPIO




/* Includes */

// Standard C libraries
#include <stdbool.h>
#include <stdint.h>




/* Defines */

// GPIO pin modes

#define LINKARCH_HAL_GPIO_MODE_INPUT  0x00
#define LINKARCH_HAL_GPIO_MODE_OUTPUT 0x01




/* Hal GPIO Functions */

bool linkarch_hal_gpio_init(uint8_t pinNumber, uint8_t pinMode);
bool linkarch_hal_gpio_read(uint8_t pinNumber);
bool linkarch_hal_gpio_put(uint8_t pinNumber, bool value);




#endif  // LINKARCH_HAL_GPIO