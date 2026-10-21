#include "macros.h"
#include <stdint.h>
#include <stdbool.h>

int main(void)
{

    // 1. Apagamos el Timer por seguridad
    REG_TCCR1A = 0x00;
    REG_TCCR1B = 0x00;
    REG_TCNT1 = 0;

    // 2. CONFIGURACIÓN DÍA 3: Definir la "pared" (El objetivo matemático)
    // Frecuencia = 16MHz / (Prescaler 1024 * 1Hz) - 1 = 15624
    REG_OCR1A = 15624;

    // 3. Encender el Modo CTC (Limpiar al coincidir)
    BIT_SET(REG_TCCR1B, BIT_WGM12);

    // 4. Encender el Prescaler a 1024 (Día 2)
    // NOTA: Al hacer esto, el Timer arranca inmediatamente.
    BIT_SET(REG_TCCR1B, BIT_CS12);
    BIT_SET(REG_TCCR1B, BIT_CS10);

    while (true)
    {
        // El bucle principal está vacío y libre de bloqueos.
        // Físicamente, el Timer está contando de 0 a 15624 una y otra vez,
        // reiniciándose exactamente cada 1.000000 segundos.
    }

    return 0;
}