add_library(linkarch_log STATIC linkarch_log.c)

# Link the project target to the libraries 
target_link_libraries(linkarch_log PUBLIC   linkarch_platform_os)

# Link to directory
target_include_directories(linkarch_log PUBLIC ./../../../include/)
target_include_directories(linkarch_log PUBLIC ./../../../include/osal)
target_include_directories(linkarch_log PUBLIC ./../../../include/hal)
target_include_directories(linkarch_log PUBLIC ./../../../include/utils/log)
target_include_directories(linkarch_log PUBLIC ./../../../include/kernel/module)
target_include_directories(linkarch_log PUBLIC ./../../../include/kernel/mode)
target_include_directories(linkarch_log PUBLIC ./../../../include/kernel/controller)
target_include_directories(linkarch_log PUBLIC ./../../../include/kernel/controller/fsm)

# Compile Optimodule
target_compile_options(linkarch_controller PRIVATE -Wall -Wextra)