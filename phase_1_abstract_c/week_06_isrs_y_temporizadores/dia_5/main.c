#include "include/macros.h"
#include <avr/interrupt.h>
#include <stdbool.h>

ISR(TIMER1_COMPA_vect)
{
    BIT_TOGGLE(PORTB, PIN_LED);
}

int main(void)
{
    /* 0. Configurar pin de LED como SALIDA (Faltó en el Spec) */
    BIT_SET(DDRB, PIN_LED);

    /* 1. Reinicio */
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    /* 2. Frecuencia (Modo CTC) */
    OCR1A = 15624;

    /* 3. Habilitacion de interrupcion local */
    BIT_SET(TIMSK1, 1); /* OCIE1A (Bit 1) */

    /* 4. Arranque del hardware */
    BIT_SET(TCCR1B, 3); /* WGM12 */
    BIT_SET(TCCR1B, 2); /* CS12 */
    BIT_SET(TCCR1B, 0); /* CS10 */

    /* 5. Interrupcion global */
    sei();

    while (true)
    {
    }

    return 0;
}
