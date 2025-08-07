if(LINKARCH_INITIALIZED)
    return()
endif()
set(LINKARCH_INITIALIZED TRUE)

set(LINKARCH_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(LINKARCH_SRC_DIR ${LINKARCH_ROOT_DIR}/src/src)
set(LINKARCH_INCLUDE_DIR ${LINKARCH_ROOT_DIR}/src/include)

include_directories(${LINKARCH_INCLUDE_DIR})

add_subdirectory(${LINKARCH_SRC_DIR} ${CMAKE_BINARY_DIR}/linkarch_build)

IF(NOT DEFINED TARGET_PLATFORM_OS)
set(TARGET_PLATFORM_OS "freertos") # ou posix, etc.
ENDIF()

if(TARGET_PLATFORM_OS STREQUAL "freertos")
    target_compile_definitions(linkarch_controller INTERFACE TARGET_OS_FREERTOS)
elseif(TARGET_PLATFORM_OS STREQUAL "posix")
    target_compile_definitions(linkarch_controller INTERFACE TARGET_OS_POSIX)
elseif(TARGET_PLATFORM_OS STREQUAL "pcConsoleDebug")
    target_compile_definitions(linkarch_controller INTERFACE TARGET_OS_PCCONSOLEDEBUG)
else()
    message(FATAL_ERROR "Unsupported TARGET_PLATFORM_OS: ${TARGET_PLATFORM_OS}")
endif()
