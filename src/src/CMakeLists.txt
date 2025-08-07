IF(NOT DEFINED TARGET_PLATFORM_MCU)
    set(TARGET_PLATFORM_MCU "picosdk") # ou posix, etc.
ENDIF()

IF(TARGET_PLATFORM_MCU STREQUAL "pcConsoleDebug")
    message(STATUS "Using platform mcu: ${TARGET_PLATFORM_MCU}")
    add_subdirectory(platform_mcu/pc_console_debug)
ELSEIF(TARGET_PLATFORM_MCU STREQUAL "picosdk")
    message(STATUS "Using platform mcu: ${TARGET_PLATFORM_MCU}")
    add_subdirectory(platform_mcu/picosdk)
ELSE()
    message(FATAL_ERROR "Unsupported TARGET_PLATFORM_MCU: ${TARGET_PLATFORM_MCU}")
ENDIF()




IF(NOT DEFINED TARGET_PLATFORM_OS)
    set(TARGET_PLATFORM_OS "freertos") # ou posix, etc.
ENDIF()

IF(TARGET_PLATFORM_OS STREQUAL "freertos")
    message(STATUS "Using platform OS: ${TARGET_PLATFORM_OS}")
    add_subdirectory(platform_os/freertos)
    add_compile_definitions(TARGET_OS_FREERTOS)
ELSEIF(TARGET_PLATFORM_OS STREQUAL "posix")
    message(STATUS "Using platform OS: ${TARGET_PLATFORM_OS}")
    add_subdirectory(platform_os/posix)
    add_compile_definitions(TARGET_OS_POSIX)
ELSEIF(TARGET_PLATFORM_OS STREQUAL "pcConsoleDebug")
    message(STATUS "Using platform OS: ${TARGET_PLATFORM_OS}")
    add_subdirectory(platform_os/pc_console_debug)
    add_compile_definitions(TARGET_OS_PCCONSOLEDEBUG)
ELSE()
    message(FATAL_ERROR "Unsupported TARGET_PLATFORM_OS: ${TARGET_PLATFORM_OS}")
ENDIF()


add_subdirectory(kernel)