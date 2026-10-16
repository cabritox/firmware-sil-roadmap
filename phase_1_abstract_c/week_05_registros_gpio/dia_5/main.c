#include "include/macros.h"
#include <stdbool.h>

int main(void)
{
    BIT_SET(REG_DDRB, PIN_LED_ESTADO);     // Definir como salida
    BIT_CLEAR(REG_DDRB, PIN_BOTON_ESTADO); // Definit como entrada

    while (true)
    {
        // Verificar si se hizo click(se intyecto voltaje) en el boton
        if (BIT_CHECK(REG_PINB, PIN_BOTON_ESTADO))
        {
            // Si se inyecto enceder led
            BIT_SET(REG_PORTB, PIN_LED_ESTADO);
        }
        else
        {
            // Si no hay paso de corriente apagar led
            BIT_CLEAR(REG_PORTB, PIN_LED_ESTADO);
        }
    }
    return 0;
}
