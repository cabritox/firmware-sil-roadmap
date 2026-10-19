#ifndef MACROS_H
#define MACROS_H

#define BIT_TOGGLE(REG, BIT) (REG ^= (1U << (BIT)))

#define REG_DDRB *(volatile uint8_t *)0x24
#define REG_PORTB *(volatile uint8_t *)0x25

#define PIN_LED_ESTADO 5

#endif 