# Add libray linkarch_module
add_library(linkarch_module STATIC linkarch_module.c)

# Link the project target to the libraries
target_link_libraries(linkarch_module PUBLIC linkarch_platform_mcu linkarch_platform_os)

# Link to directory
target_include_directories(linkarch_module PUBLIC ./../../../include/)
target_include_directories(linkarch_module PUBLIC ./../../../include/hal)
target_include_directories(linkarch_module PUBLIC ./../../../include/osal)
target_include_directories(linkarch_module PUBLIC ./../../../include/kernel/module)

# Compile Optimodule
target_compile_options(linkarch_module PRIVATE -Wall -Wextra)