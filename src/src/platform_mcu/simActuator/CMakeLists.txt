# Add libray linkarch_fsm
add_library(linkarch_platform_mcu STATIC    linkarch_platform_mcu_simActuator_clientConnection.c
                                            linkarch_platform_mcu_simActuator_clientCommand.c
                                            linkarch_platform_mcu_simActuator_spi.c
                                            linkarch_platform_mcu_simActuator_debug.c
                                            linkarch_platform_mcu_simActuator_mcuConfig.c
                                            linkarch_platform_mcu_simActuator_gpio.c
                                            linkarch_platform_mcu_simActuator_pwm.c)

# Link the project target to the libraries 
target_link_libraries(linkarch_platform_mcu PUBLIC)

# Link to directory
target_include_directories(linkarch_platform_mcu PUBLIC ./../../../include/hal/)
target_include_directories(linkarch_platform_mcu PUBLIC ./../../../include/osal/)
target_include_directories(linkarch_platform_mcu PUBLIC ./../../../include/platform_mcu/sim_actuator/)


# Compile Options
target_compile_options(linkarch_platform_mcu PRIVATE -Wall -Wextra)
