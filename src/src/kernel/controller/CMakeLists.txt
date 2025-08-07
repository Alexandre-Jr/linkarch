add_subdirectory(fsm)

# Add libray linkarch_module
add_library(linkarch_controller STATIC linkarch_controller.c)

# Link the project target to the libraries 
target_link_libraries(linkarch_controller PUBLIC    linkarch_module
                                                    linkarch_mode
                                                    linkarch_fsm
                                                    linkarch_platform_mcu)

# Link to directory
target_include_directories(linkarch_controller PUBLIC ./../../../include/)
target_include_directories(linkarch_controller PUBLIC ./../../../include/osal)
target_include_directories(linkarch_controller PUBLIC ./../../../include/hal)
target_include_directories(linkarch_controller PUBLIC ./../../../include/kernel/module)
target_include_directories(linkarch_controller PUBLIC ./../../../include/kernel/mode)
target_include_directories(linkarch_controller PUBLIC ./../../../include/kernel/controller)
target_include_directories(linkarch_controller PUBLIC ./../../../include/kernel/controller/fsm)

# Compile Optimodule
target_compile_options(linkarch_controller PRIVATE -Wall -Wextra)