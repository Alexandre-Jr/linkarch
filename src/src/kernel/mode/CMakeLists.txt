# Add libray linkarch_mode
add_library(linkarch_mode STATIC linkarch_mode.c)

# Link the project target to the libraries 
target_link_libraries(linkarch_mode PUBLIC linkarch_platform_mcu)

# Link to directory
target_include_directories(linkarch_mode PUBLIC ./../../../include/)
target_include_directories(linkarch_mode PUBLIC ./../../../include/hal)
target_include_directories(linkarch_mode PUBLIC ./../../../include/kernel/mode)

# Compile Optimodule
target_compile_options(linkarch_mode PRIVATE -Wall -Wextra)