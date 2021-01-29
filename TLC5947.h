//
// Created by tom on 1/27/21.
// Credits to Adafruit TLC5947 arduino library
// this is pretty much a direct port
//

#ifndef TEST_TLC5947_H
#define TEST_TLC5947_H

#include <cstdint>
#include <initializer_list>
#include <cstdlib>
#include "hardware/gpio.h"

class TLC5947 {

private:
    uint16_t n_drivers;
    uint8_t clk;
    uint8_t data;
    uint8_t latch;
    uint8_t blank;

    uint16_t * pwmbuf;

public:
    TLC5947(uint16_t n, uint8_t clk, uint8_t data, uint8_t latch, uint8_t blank);

    void setPWM(uint16_t chan, uint16_t pwm);
    void setLED(uint16_t lednum, uint16_t r, uint16_t g, uint16_t b);
    void write();
};


#endif //TEST_TLC5947_H
