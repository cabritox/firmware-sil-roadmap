#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h> // Libreria critica: nos da ISR() y sei()
#include "macros.h"        // Tu propia arquitectura de capas

/* ==================================================================
 * VARIABLE GLOBAL VOLATILE (La regla de oro de la memoria compartida)
 * ==================================================================
 * Si una variable se modifica ADENTRO de una ISR y se lee AFUERA (en el main),
 * DEBE llevar 'volatile'. Esto le dice al compilador: "Oye, esta variable
 * puede cambiar en cualquier nanosegundo por un fantasma (el hardware),
 * NO la guardes en caché, léela siempre desde la memoria RAM real".
 */
volatile uint8_t segundos_transcurridos = 0;

/* ==================================================================
 * LA RUTINA DE SERVICIO DE INTERRUPCIÓN (ISR)
 * ==================================================================
 * El hardware saltará aquí automáticamente.
 * Asiento en la IVT: TIMER1_COMPA_vect (Timer 1, Comparación A)
 */
ISR(TIMER1_COMPA_vect)
{
    /* Cumpliendo las 3 reglas:
       1. No hay 'return' ni parámetros.
       2. Es ultra rápido (solo manipula bits y suma).
       3. No hay delays ni whiles. */

    // Alternamos el estado del LED (Toggle) con el bisturí bitwise
    BIT_TOGGLE(REG_PORTB, PIN_LED_ESTADO);

    // Incrementamos nuestra variable contadora
    segundos_transcurridos++;
}

/* ==================================================================
 * FUNCIÓN PRINCIPAL
 * ================================================================== */
int main(void)
{
    // 1. Configurar el LED como salida
    BIT_SET(REG_DDRB, PIN_LED_ESTADO);

    // 2. Aquí iría la configuración de los registros del Timer 1
    // (Esto es lo que aprenderemos a escribir en los Días 2 y 3)
    // CONFIGURAR_TIMER1_AQUI...

    // 3. Abrir la válvula principal de interrupciones
    sei(); // sei = Set Global Interrupt Flag (Habilita el hardware de escucha)

    // 4. El Bucle Principal
    while (true)
    {
        /*
         * ¡El Polling ha muerto!
         * El procesador ya NO está preguntando "¿Ya pasó un segundo?".
         * En lugar de eso, puede hacer cálculos complejos de tu inyector,
         * o reaccionar a la variable que la ISR actualiza en las sombras:
         */

        if (segundos_transcurridos >= 60)
        {
            // Ha pasado un minuto exacto, enviar datos por UART, etc.
            segundos_transcurridos = 0; // Reiniciamos el contador
        }

        /*
         * Si no hay nada que hacer, los profesionales ponen aquí
         * una instrucción de "Sleep" para que el micro gaste 0 miliamperios
         * hasta que la próxima interrupción lo despierte.
         */
    }

    return 0;
}