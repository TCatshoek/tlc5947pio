#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "TLC5947PIO.pio.h"
#include "TLC5947.h"
#include "TLC5947Matrix.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

// GPIO defines
#define LED_PIN 25

#define ADDR1 4
#define ADDR2 3
#define ADDR3 2

#define DIN 5
#define SCLK 6
#define XLAT 7
#define BLANK 8

#define N_TLC 4

typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
} RGBValues;

RGBValues wheel(uint16_t WheelPos) {
    int scaling = 20;
    RGBValues ret;
    if(WheelPos < 1365) {
        ret.r = (3*WheelPos) / scaling;
        ret.g = (4095 - 3*WheelPos) / scaling;
        ret.b = 0;
    } else if(WheelPos < 2731) {
        WheelPos -= 1365;
        ret.r = (4095 - 3*WheelPos) / scaling;
        ret.g = 0;
        ret.b = (3*WheelPos) / scaling;
    } else {
        WheelPos -= 2731;
        ret.r = 0;
        ret.g = (3*WheelPos) / scaling;
        ret.b = (4095 - 3*WheelPos) / scaling;
    }
    return ret;
}

int main()
{
    stdio_init_all();
    
    // GPIO initialisation.
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    const uint _pins[] = { ADDR1, ADDR2, ADDR3, DIN, SCLK, XLAT, BLANK };
    for (uint pin: _pins) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
        gpio_pull_down(pin);
    }

    puts("Init TLC");

    // Create tlc5947 driver
    //TLC5947 tlc(N_TLC, SCLK, DIN, XLAT, BLANK);
    TLC5947Matrix tlc(N_TLC, SCLK, DIN, XLAT, BLANK, ADDR1, ADDR2, ADDR3);

    puts("Init success");

    absolute_time_t prev = get_absolute_time();


    uint wheel_base = 0;

    while (true) {
        absolute_time_t now = get_absolute_time();
        int64_t time_delta = absolute_time_diff_us(prev, now);

        uint wheel_value = 0;

        if (time_delta > 1 * 100) {
            for (uint col = 0; col < N_TLC * 8; col++) {
                for (uint row = 0; row < 8; row++) {
                    wheel_value = (wheel_value + 4095 / (8*8*N_TLC)) % 4095;
                    RGBValues tmp = wheel((wheel_base + wheel_value) % 4096);
                    tlc.setPixel(row, col, tmp.r, tmp.g, tmp.b);
                }
            }

            prev = now;

            wheel_base += 1;
        }

        tlc.refresh();


    }
    
    return 0;
}

#pragma clang diagnostic pop