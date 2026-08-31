cmake_minimum_required(VERSION 3.10)
project(cnc_controller)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set directories
set(BCM2835_INCLUDE_DIR "/usr/local/include")
set(BCM2835_LIBRARY "/usr/local/lib/libbcm2835.a")

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${BCM2835_INCLUDE_DIR}
)

# Source files
set(SOURCES
    src/main.cpp
    src/cnc_controller.cpp
    src/gcode_parser.cpp
    src/oled_display.cpp
    src/motor_control.cpp
    src/sd_card.cpp
    src/menu_system.cpp
    src/bcm2835_pwm.cpp
)

# Create executable
add_executable(cnc_controller ${SOURCES})

# Link libraries
target_link_libraries(cnc_controller
    ${BCM2835_LIBRARY}
    pthread
    rt
    stdc++
)

install(TARGETS cnc_controller DESTINATION /usr/local/bin)
