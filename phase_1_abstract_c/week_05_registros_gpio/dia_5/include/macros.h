#ifndef MACROS_H
#define MACROS_H

#include <stdint.h>

/*
Capa de abstraccion de hardware
*/
#define REG_PINB *(volatile uint8_t *)0x23
#define REG_DDRB *(volatile uint8_t *)0x24
#define REG_PORTB *(volatile uint8_t *)0x25

/*
 * CAPA DE ABTRACCION DE APLICACION (REGLAS DE NEGOCIO)
 */
#define PIN_LED_ESTADO 5
#define PIN_BOTON_ESTADO 0

#define BIT_SET(reg, bit) (reg |= (1U << (bit)))
#define BIT_CLEAR(reg, bit) (reg &= ~(1U << (bit)))
#define BIT_TOGGLE(reg, bit) (reg ^= (1U << (bit)))
#define BIT_CHECK(reg, bit) (reg & (1U << (bit)))

#endif