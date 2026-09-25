#include <stdio.h>
#include <stdint.h>
#include "sensor.h"

int main(void) {
    struct EstadoTermico sensor;
    sensor.temperatura = 0;
    sensor.alerta_critica = 0;

    evaluarTemperatura(&sensor, 480);

    printf("Resultado temperatura: %d | Estado critico: %d\n", sensor.temperatura, sensor.alerta_critica);

    evaluarTemperatura(NULL, 600);

    printf("\nFin del programa. Sistema a salvo.\n");
    return 0;
}