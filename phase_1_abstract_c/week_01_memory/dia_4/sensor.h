#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

struct EstadoTermico {
    uint16_t temperatura;
    uint8_t alerta_critica;
};

void evaluarTemperatura(struct EstadoTermico *estado, uint16_t lectura_adc);

#endif