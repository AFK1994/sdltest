#include "tool.h"

//×Ö½ÚÐò×ª»»
uint16_t bytes_swap_16(uint16_t value) {
	char* c = (char*)&DUAN;
	if (*c == 0x12) {
		return value;
	}
	uint16_t swapped = ((value & 0xFF00) >> 8 | (value & 0x00FF) << 8);
	return swapped;
}

uint32_t bytes_swap_32(uint32_t value) {
	char* c = (char*)&DUAN;
	if (*c == 0x12) {
		return value;
	}
	int32_t swapped = ((value & 0x000000FF) << 24 |
		(value & 0x0000FF00) << 8 |
		(value & 0x00FF0000) >> 8 |
		(value & 0xFF000000) >> 24);

	return swapped;
}

uint64_t bytes_swap_64(uint64_t value) {
	char* c = (char*)&DUAN;
	if (*c == 0x12) {
		return value;
	}
	uint64_t swapped = ((value & 0x00000000000000FF) << 56 |
		(value & 0x000000000000FF00) << 40 |
		(value & 0x0000000000FF0000) << 24 |
		(value & 0x00000000FF000000) << 8 |
		(value & 0x000000FF00000000) >> 8 |
		(value & 0x0000FF0000000000) >> 24 |
		(value & 0x00FF000000000000) >> 40 |
		(value & 0xFF00000000000000) >> 56);

	return swapped;
}