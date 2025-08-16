#ifndef LINKARCH_HAL_DEBUG
#define LINKARCH_HAL_DEBUG




// Hal debugging functions

void linkarch_hal_throwDebugMessage(const char *format, ...);




// Debugging Macros

#if LINKARCH_DEBUG_MODE
#define LINKARCH_DEBUG_MESSAGE(format, ...) \
    linkarch_controller_debug_takeDebugMessageMutex(); \
    linkarch_hal_throwDebugMessage(format "\n", ##__VA_ARGS__); \
    linkarch_controller_debug_giveDebugMessageMutex();
#define LINKARCH_DEBUG_FUNCTION(function) \
    linkarch_controller_debug_takeDebugMessageMutex(); \
    function;   \
    linkarch_controller_debug_giveDebugMessageMutex();
#else
#define LINKARCH_DEBUG_MESSAGE(message) \
    do { } while (0)
#define LINKARCH_DEBUG_FUNCTION(function) \
    do { } while (0)
#endif // LINKARCH_DEBUG_MODE

#define LINKARCH_MESSAGE(format, ...) \
    linkarch_controller_debug_takeDebugMessageMutex(); \
    linkarch_hal_throwDebugMessage(format "\n", ##__VA_ARGS__); \
    linkarch_controller_debug_giveDebugMessageMutex(); \




#endif  // LINKARCH_HAL_DEBUG