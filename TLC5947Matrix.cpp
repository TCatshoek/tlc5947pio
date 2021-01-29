//
// Created by tom on 1/27/21.
//

#include "TLC5947Matrix.h"

TLC5947Matrix::TLC5947Matrix(uint16_t n, uint8_t clk, uint8_t data, uint8_t latch, uint8_t blank,
                             uint8_t addr1, uint8_t addr2, uint8_t addr3) {

    last_refresh = get_absolute_time();

    n_chained = n;
    addr_ctr = 1;
    row_ctr = 0;

    this->addr1 = addr1;
    this->addr2 = addr2;
    this->addr3 = addr3;

    for (uint i = 0; i < N_ROWS; i++) {
        rows[i] = new TLC5947(n, clk, data, latch, blank);
    }

    for (uint pin: { addr1, addr2, addr3 }) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
        gpio_pull_down(pin);
    }
}

void TLC5947Matrix::setPixel(uint16_t row, uint16_t col, uint16_t r, uint16_t g, uint16_t b) {
    if (row > N_ROWS - 1)
        return;
    if (col > n_chained * N_ROWS)
        return;

    TLC5947* cur_row = rows[row];
    cur_row->setLED(col, r, g, b);
}

void TLC5947Matrix::refresh() {
    // Calculate the time since last refresh
    absolute_time_t now = get_absolute_time();
    int64_t time_delta = absolute_time_diff_us(last_refresh, now);

    // We aim for 60 fps, which means multiplexing a row 60 * 8 = 480 times a second
    // 1 / 60 * 8 = 0.002083...
    // so jump to a new row every 2 ms?
    if (time_delta < 2083) {
        return;
    }

    // Shift in the data for the next row
    row_ctr = (row_ctr + 1) % N_ROWS;
    rows[row_ctr]->write();

    // Select the next row
    addr_ctr += 1;
    if (addr_ctr > 8) {
        addr_ctr = 1;
    }

    gpio_put(addr1, addr_ctr & 0x01);
    gpio_put(addr2, (addr_ctr & 0x02) >> 1);
    gpio_put(addr3, (addr_ctr & 0x04) >> 2);

}

