#include "macros.h"
#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h> // Librería vital para ISR() y sei()

/*
 * LA RUTINA DE SERVICIO DE INTERRUPCIÓN (ISR)
 * La CPU saltará automáticamente aquí cada 1 segundo exacto.
 */
ISR(TIMER1_COMPA_vect)
{
    // Aquí pondremos el BIT_TOGGLE del LED en el Día 5
}

int main(void)
{

    // 1. Apagamos todo el Timer por seguridad al inicio
    REG_TCCR1A = 0x00;
    REG_TCCR1B = 0x00;
    REG_TCNT1 = 0;

    // 2. DÍA 3: Definimos la "Pared" matemática para 1Hz
    REG_OCR1A = 15624;

    // 3. DÍA 4: Le quitamos el bozal al Temporizador (Interruptor Local)
    // Le decimos: "Cuando TCNT1 == OCR1A, genera una señal eléctrica".
    BIT_SET(REG_TIMSK1, BIT_OCIE1A);

    // 4. DÍAS 2 y 3: Encendemos el hardware (Modo CTC y Prescaler 1024)
    // En el instante en que estas líneas se ejecutan, el Timer empieza a contar.
    BIT_SET(REG_TCCR1B, BIT_WGM12);
    BIT_SET(REG_TCCR1B, BIT_CS12);
    BIT_SET(REG_TCCR1B, BIT_CS10);

    // 5. DÍA 4: Le quitamos los auriculares a la CPU (Interruptor Global)
    // Le decimos: "Ya puedes escuchar las alarmas de los periféricos".
    sei();

    // 6. El Bucle Principal
    while (true)
    {
        // La CPU está completamente libre. El hardware se encarga del tiempo.
    }

    return 0;
}