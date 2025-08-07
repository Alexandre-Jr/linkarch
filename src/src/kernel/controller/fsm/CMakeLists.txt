# Add libray linkarch_fsm
add_library(linkarch_fsm STATIC linkarch_fsm.c)

# Link the project target to the libraries 
target_link_libraries(linkarch_fsm PUBLIC linkarch_platform_mcu)

# Link to directory
target_include_directories(linkarch_fsm PUBLIC ./../../../../include/hal)
target_include_directories(linkarch_fsm PUBLIC ./../../../../include/kernel/controller/fsm)

# Compile Options
target_compile_options(linkarch_fsm PRIVATE -Wall -Wextra)