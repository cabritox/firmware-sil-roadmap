#include "sensor.h"
#include <stddef.h>
#include <stdio.h>

void evaluarTemperatura(struct EstadoTermico *estado, uint16_t lectura_adc){

    if(estado == NULL){
        printf("ALERTA: Intento de escritura en direccion nula.");
        return;
    }

    estado->temperatura = lectura_adc / 10;

    if (estado->temperatura >= 45){
        estado->alerta_critica = 1;
    }else{
        estado->alerta_critica = 0;
    }

}