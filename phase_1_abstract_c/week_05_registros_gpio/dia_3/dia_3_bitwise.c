#include <stdint.h>
#include <stdbool.h>

/* Direcciones fisicas mapeadas a memoria (ATmega328P Puerto B) */
#define REGISTRO_PINB *(volatile uint8_t *)0x23
#define REGISTRO_DDRB *(volatile uint8_t *)0x24
#define REGISTRO_PORTB *(volatile uint8_t *)0x25

int main(void)
{
    /* 1. CONFIGURACION (DDRB) */
    /* Configuramos PB5 como SALIDA (LED) usando SET */
    REGISTRO_DDRB |= (1U << 5);

    /* Configuramos PB0 como ENTRADA (Boton).
       Para entrada forzamos un 0 usando CLEAR */
    REGISTRO_DDRB &= ~(1U << 0);

    /* 2. BUCLE INFINITO DE HARDWARE */
    while (true)
    {

        /* 3. CHECK: Leemos el estado fisico del boton en PB0 */
        if (REGISTRO_PINB & (1U << 0))
        {

            /* Si el boton esta presionado (HIGH), SETeamos el LED */
            REGISTRO_PORTB |= (1U << 5);
        }
        else
        {

            /* Si esta suelto (LOW), CLEAReamos el LED */
            REGISTRO_PORTB &= ~(1U << 5);
        }

        /* Nota: Para hacer que el LED parpadee sin depender del boton,
           simplemente usariamos: REGISTRO_PORTB ^= (1U << 5); */
    }

    return 0;
}