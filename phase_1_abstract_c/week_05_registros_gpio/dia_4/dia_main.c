#include "dia_macros.h"
#include <stdbool.h>

int main(void)
{
    /* 1. INICIALIZACION DEL SISTEMA */
    /* El codigo se lee casi como un idioma natural */
    BIT_SET(REG_DDRB, PIN_LED_ESTADO);     // Configurar LED como Salida
    BIT_CLEAR(REG_DDRB, PIN_BOTON_SENSOR); // Configurar Boton como Entrada

    /* 2. BUCLE INFINITO DE CONTROL */
    while (true)
    {

        /* Monitorear el sensor del mundo exterior */
        if (BIT_CHECK(REG_PINB, PIN_BOTON_SENSOR))
        {

            /* Accion: Encender la luz de estado */
            BIT_SET(REG_PORTB, PIN_LED_ESTADO);
        }
        else
        {

            /* Accion: Apagar la luz de estado */
            BIT_CLEAR(REG_PORTB, PIN_LED_ESTADO);
        }
    }

    return 0;
}