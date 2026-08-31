#ifndef BCM2835_PWM_H
#define BCM2835_PWM_H

#include <bcm2835.h>

class BCM2835PWM {
public:
    BCM2835PWM(uint8_t pin);
    ~BCM2835PWM();
    
    bool initialize();
    void setDutyCycle(float percentage);
    void setFrequency(uint32_t frequency);
    void enable();
    void disable();
    
private:
    uint8_t m_pin;
    uint8_t m_channel;
    bool m_initialized;
    uint32_t m_clock_div;
    uint32_t m_range;
};

#endif
