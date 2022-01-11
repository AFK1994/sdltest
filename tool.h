#pragma once
#include <fstream>
#include <iostream>

const uint8_t DUAN = 0x1234;

uint16_t bytes_swap_16(uint16_t int_value);

uint32_t bytes_swap_32(uint32_t int_value);

uint64_t bytes_swap_64(uint64_t int_value);
