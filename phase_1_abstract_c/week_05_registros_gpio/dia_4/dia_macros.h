#ifndef DIA_4_MACROS_H
#define DIA_4_MACROS_H

#include <stdint.h>

/* ==================================================================
 * CAPA 1: ABSTRACCION DE HARDWARE (Direcciones fisicas)
 * ================================================================== */
#define REG_PORTB *(volatile uint8_t *)0x25
#define REG_DDRB *(volatile uint8_t *)0x24
#define REG_PINB *(volatile uint8_t *)0x23

/* ==================================================================
 * CAPA 2: ABSTRACCION DE APLICACION (Reglas de negocio)
 * ================================================================== */
/* Aqui conectamos el pin fisico con su proposito en el mundo real */
#define PIN_LED_ESTADO 5   // Conectado al pin fisico PB5
#define PIN_BOTON_SENSOR 0 // Conectado al pin fisico PB0

/* ==================================================================
 * CAPA 3: MACROS DE ACCION (El Bisturi seguro con 1UL y parentesis)
 * ================================================================== */
#define BIT_SET(reg, bit) ((reg) |= (1UL << (bit)))
#define BIT_CLEAR(reg, bit) ((reg) &= ~(1UL << (bit)))
#define BIT_TOGGLE(reg, bit) ((reg) ^= (1UL << (bit)))
#define BIT_CHECK(reg, bit) ((reg) & (1UL << (bit)))

#endif /* DIA_4_MACROS_H */