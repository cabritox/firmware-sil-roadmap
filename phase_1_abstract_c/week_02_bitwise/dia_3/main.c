#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "macros.h"

int main(void)
{
    /* Simulamos el registro de control de un motor (8 bits) */
    uint8_t ctrl_motor = 0;

    /* 1. Encendemos el motor (Pin 2) y la alarma (Pin 7) usando la HAL */
    SET_BIT(ctrl_motor, 2);
    SET_BIT(ctrl_motor, 7);

    /* Exigimos matemáticamente que los bits estén en 1 */
    assert(READ_BIT(ctrl_motor, 2) == 1);
    assert(READ_BIT(ctrl_motor, 7) == 1);
    assert(READ_BIT(ctrl_motor, 0) == 0); /* El pin 0 no debería haber sido tocado */

    /* 2. Apagamos la alarma (Pin 7) */
    CLEAR_BIT(ctrl_motor, 7);
    assert(READ_BIT(ctrl_motor, 7) == 0);
    assert(READ_BIT(ctrl_motor, 2) == 1); /* El motor debe seguir encendido */

    /* 3. Alternamos el estado de un LED de estado (Pin 0) */
    TOGGLE_BIT(ctrl_motor, 0);
    assert(READ_BIT(ctrl_motor, 0) == 1);
    TOGGLE_BIT(ctrl_motor, 0);
    assert(READ_BIT(ctrl_motor, 0) == 0);

    printf("HAL validada exitosamente. Abstraccion de hardware operativa.\n");
    return 0;
}