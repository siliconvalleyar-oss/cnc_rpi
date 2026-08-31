#ifndef CONFIG_H
#define CONFIG_H

#include <bcm2835.h>

// Pin definitions for Raspberry Pi 2W (BCM2835 numbering)
// Motor step/dir pins (Pololu drivers)
#define X_STEP_PIN    RPI_GPIO_P1_11  // GPIO17
#define X_DIR_PIN     RPI_GPIO_P1_13  // GPIO27
#define Y_STEP_PIN    RPI_GPIO_P1_15  // GPIO22
#define Y_DIR_PIN     RPI_GPIO_P1_16  // GPIO23
#define Z_STEP_PIN    RPI_GPIO_P1_18  // GPIO24
#define Z_DIR_PIN     RPI_GPIO_P1_22  // GPIO25
#define A_STEP_PIN    RPI_GPIO_P1_29  // GPIO5
#define A_DIR_PIN     RPI_GPIO_P1_31  // GPIO6

// Endstop pins (active low)
#define X_LIMIT_PIN   RPI_GPIO_P1_19  // GPIO10
#define Y_LIMIT_PIN   RPI_GPIO_P1_21  // GPIO9
#define Z_LIMIT_PIN   RPI_GPIO_P1_23  // GPIO11
#define A_LIMIT_PIN   RPI_GPIO_P1_24  // GPIO8

// Spindle PWM pin (hardware PWM on GPIO12)
#define SPINDLE_PWM_PIN RPI_GPIO_P1_32  // GPIO12

// OLED display (I2C1 - pins GPIO2, GPIO3)
#define OLED_I2C_ADDR  0x3C

// SD Card (SPI0)
#define SD_CS_PIN     RPI_GPIO_P1_26  // GPIO7

// Buttons (USB keyboard)
#define BUTTON_UP      1
#define BUTTON_DOWN    2
#define BUTTON_LEFT    3
#define BUTTON_RIGHT   4
#define BUTTON_ENTER   5
#define BUTTON_ESC     6

// Motor parameters
#define MOTOR_STEPS_PER_REV 200
#define LEAD_SCREW_PITCH    8.0
#define STEPS_PER_MM (MOTOR_STEPS_PER_REV / LEAD_SCREW_PITCH)
#define MICROSTEPPING 1
#define STEPS_PER_MM_FINAL (STEPS_PER_MM * MICROSTEPPING)

// Maximum speeds (mm/min)
#define MAX_FEEDRATE_X 1000.0
#define MAX_FEEDRATE_Y 1000.0
#define MAX_FEEDRATE_Z 500.0
#define MAX_FEEDRATE_A 360.0

#define DEFAULT_ACCELERATION 100.0

// Spindle speed range
#define SPINDLE_MIN_SPEED  0
#define SPINDLE_MAX_SPEED  10000

#define GCODE_BUFFER_SIZE 256
#define COMMAND_BUFFER_SIZE 100

#endif
