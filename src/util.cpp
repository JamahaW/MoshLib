#include "util.hpp"

extern unsigned int __bss_end;
extern void* __brkval;

uint16_t getFreeSram() {
    uint8_t v;
    return ((uint16_t) __brkval == 0) ? (((uint16_t) &v) - ((uint16_t) &__bss_end)) : (((uint16_t) &v) - ((uint16_t) __brkval));
};