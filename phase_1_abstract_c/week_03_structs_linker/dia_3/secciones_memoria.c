#include <stdio.h>
#include <stdint.h>

/* 1. .rodata: Constante global de solo lectura */
const uint8_t TABLA_CALIBRACION[8] = {10, 20, 30, 40, 50, 60, 70, 80};

/* 2. .data: Variable global inicializada con valor != 0 */
uint32_t bms_heartbeat_counter = 0xDEADBEEF;

/* 3. .bss: Buffer global sin inicializar (o en 0) */
uint8_t buffer_muestreo_raw[1024];

/* 4. SECCION PERSONALIZADA: Atributo de compilador */
/* Le decimos al linker que no la ponga en .data, sino en una caja especial */
__attribute__((section(".eeprom_emulada")))
uint32_t configuracion_guardada = 0x11223344;

int main(void)
{
    /* Las variables locales (como 'ahora') NO van a estas secciones, van al Stack (Pila) en tiempo de ejecucion */
    uint32_t ahora = 100;

    printf("==================================================\n");
    printf("   MAPA DE MEMORIA VIRTUAL (UBICACION DE SIMBOLOS)\n");
    printf("==================================================\n");

    printf(".rodata (TABLA_CALIBRACION):     %p\n", (void *)TABLA_CALIBRACION);
    printf(".data   (bms_heartbeat_counter): %p\n", (void *)&bms_heartbeat_counter);
    printf(".bss    (buffer_muestreo_raw):   %p\n", (void *)buffer_muestreo_raw);
    printf("Custom  (configuracion_guardada):%p\n", (void *)&configuracion_guardada);
    printf("Stack   (variable local 'ahora'):%p\n", (void *)&ahora);

    return 0;
}