#ifndef DIA_1_DATASHEET_H
#define DIA_1_DATASHEET_H

/*
 * TRADUCCION DEL DATASHEET A C (Mapa de Memoria Bare-Metal)
 * Microcontrolador: ATmega328P (Arquitectura AVR 8-bits)
 *
 * En lugar de usar la magia oculta de la libreria de Arduino,
 * declaramos las direcciones fisicas exactas de los registros
 * que controlan el Puerto B (Pines PB0 a PB7).
 */

/* 1. Direccion base y registros del Puerto B */
#define DIRECCION_FISICA_PINB 0x23  /* Registro para LEER el voltaje de los pines */
#define DIRECCION_FISICA_DDRB 0x24  /* Registro para CONFIGURAR si es Entrada (0) o Salida (1) */
#define DIRECCION_FISICA_PORTB 0x25 /* Registro para ESCRIBIR voltaje HIGH (1) o LOW (0) */

#endif /* DIA_1_DATASHEET_H */