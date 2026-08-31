/**
 * @file bcm2835.h
 * @brief Stub/Mock de la librería bcm2835 para verificación de compilación.
 *
 * Este archivo proporciona definiciones y declaraciones mínimas para permitir
 * la compilación y verificación de sintaxis en sistemas sin bcm2835 instalada.
 *
 * NOTA: Este archivo es SOLO para desarrollo/verificación. En una Raspberry Pi real,
 * se debe instalar la librería bcm2835 real mediante scripts/install_deps.sh
 */

#ifndef BCM2835_STUB_H
#define BCM2835_STUB_H

#include <cstdint>
#include <cstdio>

// ============================================================================
// Tipos de datos
// ============================================================================

typedef uint8_t uint8_t;
typedef uint16_t uint16_t;
typedef uint32_t uint32_t;
typedef uint64_t uint64_t;

// ============================================================================
// Constantes GPIO
// ============================================================================

#define HIGH 1
#define LOW  0

// Funciones alternativas de GPIO
#define BCM2835_GPIO_FSEL_INPT  0
#define BCM2835_GPIO_FSEL_OUTP  1
#define BCM2835_GPIO_FSEL_ALT0  4
#define BCM2835_GPIO_FSEL_ALT1  5
#define BCM2835_GPIO_FSEL_ALT2  6
#define BCM2835_GPIO_FSEL_ALT3  7
#define BCM2835_GPIO_FSEL_ALT4  3
#define BCM2835_GPIO_FSEL_ALT5  2

// Pull-up/down
#define BCM2835_GPIO_PUD_OFF  0
#define BCM2835_GPIO_PUD_DOWN 1
#define BCM2835_GPIO_PUD_UP   2

// Pines RPI (compatibilidad con código legacy)
#define RPI_GPIO_P1_03  2
#define RPI_GPIO_P1_05  3
#define RPI_GPIO_P1_07  4
#define RPI_GPIO_P1_08  14
#define RPI_GPIO_P1_10  15
#define RPI_GPIO_P1_11  17
#define RPI_GPIO_P1_12  18
#define RPI_GPIO_P1_13  27
#define RPI_GPIO_P1_15  22
#define RPI_GPIO_P1_16  23
#define RPI_GPIO_P1_18  24
#define RPI_GPIO_P1_19  10
#define RPI_GPIO_P1_21  9
#define RPI_GPIO_P1_22  25
#define RPI_GPIO_P1_23  11
#define RPI_GPIO_P1_24  8
#define RPI_GPIO_P1_26  7
#define RPI_GPIO_P1_29  5
#define RPI_GPIO_P1_31  6
#define RPI_GPIO_P1_32  12
#define RPI_GPIO_P1_33  13
#define RPI_GPIO_P1_35  19
#define RPI_GPIO_P1_36  16
#define RPI_GPIO_P1_37  26
#define RPI_GPIO_P1_38  20
#define RPI_GPIO_P1_40  21

// PWM
#define BCM2835_PWM_CLOCK_DIVIDER_1   1
#define BCM2835_PWM_CLOCK_DIVIDER_16  16

// ============================================================================
// Funciones stub (no hacen nada pero permiten compilación)
// ============================================================================

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Inicializa bcm2835 (stub).
 * @return 1 si "exitoso".
 */
static inline int bcm2835_init(void) {
    printf("[STUB] bcm2835_init()\n");
    return 1;
}

/**
 * @brief Cierra bcm2835 (stub).
 */
static inline void bcm2835_close(void) {
    printf("[STUB] bcm2835_close()\n");
}

/**
 * @brief Selecciona función de GPIO (stub).
 */
static inline void bcm2835_gpio_fsel(uint8_t pin, uint8_t mode) {
    printf("[STUB] bcm2835_gpio_fsel(pin=%d, mode=%d)\n", pin, mode);
}

/**
 * @brief Escribe en GPIO (stub).
 */
static inline void bcm2835_gpio_write(uint8_t pin, uint8_t on) {
    printf("[STUB] bcm2835_gpio_write(pin=%d, on=%d)\n", pin, on);
}

/**
 * @brief Lee GPIO (stub).
 * @return 0 (LOW).
 */
static inline uint8_t bcm2835_gpio_lev(uint8_t pin) {
    return LOW;
}

/**
 * @brief Configura pull-up/down (stub).
 */
static inline void bcm2835_gpio_set_pud(uint8_t pin, uint8_t pud) {
    printf("[STUB] bcm2835_gpio_set_pud(pin=%d, pud=%d)\n", pin, pud);
}

/**
 * @brief Delay en microsegundos (stub).
 */
static inline void bcm2835_delayMicroseconds(uint64_t micros) {
    // En stub no hacemos delay real
}

/**
 * @brief Delay en milisegundos (stub).
 */
static inline void bcm2835_delay(uint32_t millis) {
    // En stub no hacemos delay real
}

// ============================================================================
// Funciones PWM stub
// ============================================================================

static inline void bcm2835_pwm_set_clock(uint32_t divisor) {
    printf("[STUB] bcm2835_pwm_set_clock(%u)\n", divisor);
}

static inline void bcm2835_pwm_set_mode(uint8_t channel, uint8_t markspace, uint8_t enabled) {
    printf("[STUB] bcm2835_pwm_set_mode(ch=%d, ms=%d, en=%d)\n", channel, markspace, enabled);
}

static inline void bcm2835_pwm_set_range(uint8_t channel, uint32_t range) {
    printf("[STUB] bcm2835_pwm_set_range(ch=%d, range=%u)\n", channel, range);
}

static inline void bcm2835_pwm_set_data(uint8_t channel, uint32_t data) {
    printf("[STUB] bcm2835_pwm_set_data(ch=%d, data=%u)\n", channel, data);
}

// ============================================================================
// Funciones I2C stub
// ============================================================================

static inline void bcm2835_i2c_begin(void) {
    printf("[STUB] bcm2835_i2c_begin()\n");
}

static inline void bcm2835_i2c_end(void) {
    printf("[STUB] bcm2835_i2c_end()\n");
}

static inline void bcm2835_i2c_setSlaveAddress(uint8_t addr) {
    printf("[STUB] bcm2835_i2c_setSlaveAddress(0x%02X)\n", addr);
}

static inline uint8_t bcm2835_i2c_write(const void* buf, uint32_t len) {
    printf("[STUB] bcm2835_i2c_write(len=%u)\n", len);
    return 0;
}

// ============================================================================
// Funciones SPI stub
// ============================================================================

static inline void bcm2835_spi_begin(void) {
    printf("[STUB] bcm2835_spi_begin()\n");
}

static inline void bcm2835_spi_end(void) {
    printf("[STUB] bcm2835_spi_end()\n");
}

static inline void bcm2835_spi_setBitOrder(uint8_t order) {}
static inline void bcm2835_spi_setDataMode(uint8_t mode) {}
static inline void bcm2835_spi_setClockDivider(uint16_t divider) {}
static inline void bcm2835_spi_chipSelect(uint8_t cs) {}
static inline void bcm2835_spi_setChipSelectPolarity(uint8_t cs, uint8_t active) {}
static inline uint8_t bcm2835_spi_transfer(uint8_t value) { return 0; }
static inline void bcm2835_spi_transfernb(const char* tbuf, char* rbuf, uint32_t len) {}
static inline void bcm2835_spi_transfern(char* buf, uint32_t len) {}
static inline void bcm2835_spi_writenb(const char* buf, uint32_t len) {}

#ifdef __cplusplus
}
#endif

#endif // BCM2835_STUB_H
