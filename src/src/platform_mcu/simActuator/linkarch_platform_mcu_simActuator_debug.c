#include "linkarch_hal_debug.h"

// Standard C libraries
#include <stdio.h>
#include <stdarg.h>




void linkarch_hal_throwDebugMessage(const char *format, ...) {
    
    if (format == NULL) {
        printf("DEBUG: NULL format string\n");
    }

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

}
