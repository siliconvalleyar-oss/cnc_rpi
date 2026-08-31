#include "bcm2835_pwm.h"

BCM2835PWM::BCM2835PWM(uint8_t pin) 
    : m_pin(pin), m_channel(0), m_initialized(false), 
      m_clock_div(BCM2835_PWM_CLOCK_DIVIDER_16), m_range(1024) {
    
    if(pin == RPI_GPIO_P1_12 || pin == RPI_GPIO_P1_32) {
        m_channel = 0;
    } else if(pin == RPI_GPIO_P1_33 || pin == RPI_GPIO_P1_35) {
        m_channel = 1;
    }
}

BCM2835PWM::~BCM2835PWM() {
    disable();
}

bool BCM2835PWM::initialize() {
    if(!bcm2835_init()) {
        return false;
    }
    
    if(m_pin == RPI_GPIO_P1_12 || m_pin == RPI_GPIO_P1_32) {
        bcm2835_gpio_fsel(m_pin, BCM2835_GPIO_FSEL_ALT0);
    } else if(m_pin == RPI_GPIO_P1_33 || m_pin == RPI_GPIO_P1_35) {
        bcm2835_gpio_fsel(m_pin, BCM2835_GPIO_FSEL_ALT0);
    }
    
    bcm2835_pwm_set_clock(m_clock_div);
    bcm2835_pwm_set_mode(m_channel, 1, 1);
    bcm2835_pwm_set_range(m_channel, m_range);
    
    m_initialized = true;
    return true;
}

void BCM2835PWM::setDutyCycle(float percentage) {
    if(!m_initialized) return;
    if(percentage < 0) percentage = 0;
    if(percentage > 100) percentage = 100;
    
    uint32_t data = (percentage / 100.0) * m_range;
    bcm2835_pwm_set_data(m_channel, data);
}

void BCM2835PWM::setFrequency(uint32_t frequency) {
    if(!m_initialized) return;
    uint32_t divider = 19200000 / (frequency * m_range);
    if(divider < 2) divider = 2;
    if(divider > 4095) divider = 4095;
    m_clock_div = divider;
    bcm2835_pwm_set_clock(m_clock_div);
}

void BCM2835PWM::enable() {
    if(!m_initialized) return;
    bcm2835_pwm_set_mode(m_channel, 1, 1);
}

void BCM2835PWM::disable() {
    if(!m_initialized) return;
    bcm2835_pwm_set_mode(m_channel, 0, 0);
}
