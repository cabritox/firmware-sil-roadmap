#include <stdint.h>

/* 1. Definimos las macros de hardware usando nuestra nueva "jeringa" de punteros */
#define REGISTRO_DDRB *(volatile uint8_t *)0x24  /* Configurar entrada/salida */
#define REGISTRO_PORTB *(volatile uint8_t *)0x25 /* Emitir voltaje */

int main(void)
{
    /*
     * MISION 1: Configurar el pin PB5 como SALIDA.
     * El registro DDRB tiene 8 bits (PB7, PB6, PB5, PB4, PB3, PB2, PB1, PB0).
     * Para que PB5 sea salida, su bit debe ser '1'.
     * En binario: 00100000
     */
    REGISTRO_DDRB = 0b00100000;

    /*
     * MISION 2: Encender el LED conectado a PB5 (Mandar 5 Voltios).
     * El registro PORTB escupe voltaje. Mandamos un '1' al mismo bit.
     */
    REGISTRO_PORTB = 0b00100000 | 0b00000001;

    /* En este exacto milisegundo, la luz fisica se enciende en el mundo real. */

    while (1)
    {
        /* Bucle infinito para evitar que el microcontrolador se apague */
    }

    return 0;
}