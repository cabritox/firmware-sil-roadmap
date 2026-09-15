#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t buffer_rx[1000000]; /* 1. Memoria Estatica (Sección .bss) */

void funcion_sensor(void) {
    uint8_t sensor_data[10]; /* Stack secundario */
    printf("[Stack] sensor_data:  %p\n", (void*)&sensor_data);
}

int main(void) {
    uint8_t main_var = 5; /* Stack principal */
    
    /* 3. Heap (Monticulo dinamico) */
    uint8_t *payload = malloc(512); 

    printf("=== MAPA DE MEMORIA ===\n");
    printf("[Estatica] buffer_rx: %p\n", (void*)&buffer_rx);
    printf("[Heap] payload:       %p\n", (void*)payload);
    printf("[Stack] main_var:     %p\n", (void*)&main_var);
    
    funcion_sensor();

    free(payload);
    return 0;
}