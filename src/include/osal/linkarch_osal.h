#ifndef LINKARCH_OLSA
#define LINKARCH_OLSA




// Types
#if defined(TARGET_OS_POSIX)
    #include "platform_os/posix/linkarch_platform_os_posix.h"
#elif defined(TARGET_OS_PCCONSOLEDEBUG)
    #include "platform_os/pc_console_debug/linkarch_platform_os_pcConsoleDebug.h"
#elif defined(TARGET_OS_FREERTOS)
    #include "platform_os/freertos/linkarch_platform_os_freertos.h"
#else
    #error "Unsupported OS platform"
#endif




#endif  // LINKARCH_OLSA