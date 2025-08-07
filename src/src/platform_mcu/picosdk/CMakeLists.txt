# Add libray linkarch_fsm
add_library(linkarch_platform_mcu STATIC linkarch_platform_mcu_picosdk_debug.c
                                            linkarch_platform_mcu_picosdk_mcuConfig.c)

# Link the project target to the libraries 
target_link_libraries(linkarch_platform_mcu PUBLIC pico_stdlib)

# Link to directory
target_include_directories(linkarch_platform_mcu PUBLIC ./../../../include/hal/)

# Compile Options
target_compile_options(linkarch_platform_mcu PRIVATE -Wall -Wextra)
