//
// Created by tom on 1/27/21.
// Credits to Adafruit TLC5947 arduino library
// this is pretty much a direct port
//

#include "TLC5947.h"

TLC5947::TLC5947(uint16_t n, uint8_t clk, uint8_t data, uint8_t latch, uint8_t blank) {
    this->n_drivers = n;
    this->clk = clk;
    this->data = data;
    this->latch = latch;
    this->blank = blank;

    for (uint pin: { clk, data, latch, blank }) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
        gpio_pull_down(pin);
    }

    pwmbuf = (uint16_t *)malloc(2 * 24 * n);
}

void TLC5947::setPWM(uint16_t chan, uint16_t pwm) {
    if (pwm > 4095)
        pwm = 4095;
    if (chan > 24 * n_drivers)
        return;
    pwmbuf[chan] = pwm;
}

void TLC5947::setLED(uint16_t lednum, uint16_t r, uint16_t g, uint16_t b) {
    setPWM(lednum * 3, r);
    setPWM(lednum * 3 + 1, g);
    setPWM(lednum * 3 + 2, b);
}

void TLC5947::write() {
    gpio_put(latch, 0);
    // 24 channels per TLC5974
    for (int16_t c = 24 * n_drivers - 1; c >= 0; c--) {
        // 12 bits per channel, send MSB first
        for (int8_t b = 11; b >= 0; b--) {
            gpio_put(clk, 0);

            if (pwmbuf[c] & (1 << b))
                gpio_put(data, 1);
            else
                gpio_put(data, 0);

            gpio_put(clk, 1);
        }
    }
    gpio_put(clk, 0);

    gpio_put(blank, 1);
    gpio_put(latch, 1);
    gpio_put(latch, 0);
    gpio_put(blank, 0);
}


