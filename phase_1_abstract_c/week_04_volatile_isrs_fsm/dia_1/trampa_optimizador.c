#include <stdint.h>

/* Bandera que supuestamente cambiará una interrupción de hardware externa */
volatile uint8_t flag_sensor_listo = 0;

void esperar_sensor(void)
{
    /* El microcontrolador se queda esperando a que el sensor avise */
    while (flag_sensor_listo == 0)
    {
        /* Bucle de polling (espera activa) */
    }
}