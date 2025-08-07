# Add libray linkarch_fsm
add_library(linkarch_platform_os STATIC linkarch_platform_os_freertos.c)

# Link the project target to the libraries 
target_link_libraries(linkarch_platform_os PUBLIC FreeRTOS-Kernel-Heap4)

# Link to directory
target_include_directories(linkarch_platform_os PUBLIC ./../../../include/platform_os/freertos)
target_include_directories(linkarch_platform_os PUBLIC ${FREERTOS_CONFIG_PATH})

# Compile Options
target_compile_options(linkarch_platform_os PRIVATE -Wall -Wextra)
