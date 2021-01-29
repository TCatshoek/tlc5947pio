//
// Created by tom on 1/27/21.
//

#ifndef TEST_TLC5947MATRIX_H
#define TEST_TLC5947MATRIX_H

#include <cstdint>
#include <pico/time.h>
#include "TLC5947.h"

#define N_ROWS 8

class TLC5947Matrix {
private:
    TLC5947* rows[N_ROWS];
    uint n_chained;

    uint8_t addr_ctr;
    uint8_t row_ctr;
    absolute_time_t last_refresh;

    uint8_t addr1;
    uint8_t addr2;
    uint8_t addr3;

public:
    TLC5947Matrix(uint16_t n, uint8_t clk, uint8_t data, uint8_t latch, uint8_t blank,
                  uint8_t addr1, uint8_t addr2, uint8_t addr3);
    void setPixel(uint16_t row, uint16_t col, uint16_t r, uint16_t g, uint16_t b);
    void refresh();
};


#endif //TEST_TLC5947MATRIX_H
