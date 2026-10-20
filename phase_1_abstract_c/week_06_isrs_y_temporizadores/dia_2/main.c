#include "macros.h"
#include <stdint.h>
#include <stdbool.h>

int main(void)
{

    // 1. Apagamos el Timer 1 completamente por seguridad antes de configurarlo
    REG_TCCR1A = 0x00;
    REG_TCCR1B = 0x00;
    REG_TCNT1 = 0; // Reiniciamos el cuentakilómetros a cero

    // 2. Encendemos el motor del Timer conectando el Prescaler a 1024
    // Usamos el bisturí bitwise que creaste en la semana 5
    BIT_SET(REG_TCCR1B, BIT_CS12);
    BIT_SET(REG_TCCR1B, BIT_CS10);

    /*
     * ¡A partir de la línea anterior, el hardware del Timer 1 ya está vivo!
     * Empezó a sumar 1 al registro REG_TCNT1 cada 64 microsegundos
     * (15,625 tics por segundo) en segundo plano, sin que tu while se entere.
     */

    while (true)
    {
        // En este punto (Día 2), si quisieras ver cómo el temporizador cuenta,
        // podrías leer el registro físico así:
        // uint16_t tics_actuales = REG_TCNT1;

        // (Pero hacer esto sería volver a caer en el pecado del "polling")
    }

    return 0;
}