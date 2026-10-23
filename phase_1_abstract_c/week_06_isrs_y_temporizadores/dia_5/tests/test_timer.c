#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "include/macros.h"

static void test_bit_macros(void)
{
    uint8_t r = 0;
    BIT_SET(r, 3);
    assert(r == (1U << 3));
    BIT_CLEAR(r, 3);
    assert(r == 0);
    BIT_TOGGLE(r, 1);
    assert(r == (1U << 1));
    BIT_TOGGLE(r, 1);
    assert(r == 0);
    printf("[PASS] bit macros\n");
}

static void test_register_addresses(void)
{
    assert((uintptr_t)&PORTB == 0x25);
    assert((uintptr_t)&DDRB == 0x24);
    assert((uintptr_t)&TCCR1A == 0x80);
    assert((uintptr_t)&TCCR1B == 0x81);
    assert((uintptr_t)&TIMSK1 == 0x6F);
    assert((uintptr_t)&TCNT1 == 0x84);
    assert((uintptr_t)&OCR1A == 0x88);
    assert(PIN_LED == 5);
    printf("[PASS] register addresses\n");
}

int main(void)
{
    test_bit_macros();
    test_register_addresses();
    printf("ALL TESTS PASSED\n");
    return 0;
}
