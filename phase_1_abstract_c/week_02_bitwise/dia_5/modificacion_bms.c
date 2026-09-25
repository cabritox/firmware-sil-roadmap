#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/* =========================================================================
 * 1. CAPA HAL: Motor Completo de Manipulación de Campos (Días 3, 5 y 6)
 * ========================================================================= */
#define BIT_MASK(ANCHO) (((ANCHO) >= 32U) ? 0xFFFFFFFFUL : ((1UL << (ANCHO)) - 1UL))

#define PACK_FIELD(VALOR, ANCHO, POSICION) \
    (((uint32_t)(VALOR) & BIT_MASK(ANCHO)) << (POSICION))

#define UNPACK_FIELD(TRAMA, POSICION, ANCHO) \
    (((TRAMA) >> (POSICION)) & BIT_MASK(ANCHO))

/* TODO 1: Define la macro CLEAR_FIELD(REG, ANCHO, POSICION)
 * Debe devolver el valor de REG pero con los bits del carril puestos en 0.
 * Pista: Usa (REG) & ~((uint32_t)BIT_MASK(ANCHO) << (POSICION))
 */
#define CLEAR_FIELD(REG, ANCHO, POSICION) ((REG) & ~((uint32_t)BIT_MASK(ANCHO) << (POSICION)))

/* TODO 2: Define la macro MODIFY_FIELD(REG, ANCHO, POSICION, VALOR)
 * Primero limpia el carril usando CLEAR_FIELD y luego une (|) el nuevo dato usando PACK_FIELD.
 */
#define MODIFY_FIELD(REG, ANCHO, POSICION, VALOR) (CLEAR_FIELD(REG, ANCHO, POSICION)) | PACK_FIELD(VALOR, ANCHO, POSICION)

int main(void)
{
    /* Estado inicial en vivo del registro (Celda #3 | Flags: 0x09 | Voltaje: 4250 mV) */
    uint32_t registro_bms = 0x0109A903U;
    printf("[INICIO] Registro en vivo         : 0x%08X\n", registro_bms);

    /* --- PRUEBA 1: Limpieza quirúrgica de un carril --- */
    /* Vaciamos únicamente el carril de flags_error (Ancho: 4 bits, Posición: 8) */
    uint32_t registro_sin_flags = CLEAR_FIELD(registro_bms, 4, 8);
    printf("[PASO 1] Carril de flags vaciado  : 0x%08X\n", registro_sin_flags);

    /* El '9' en la tercera posición debió convertirse en '0' sin tocar el resto */
    assert(registro_sin_flags == 0x0109A003U);

    /* --- PRUEBA 2: Actualización en caliente (Read-Modify-Write) --- */
    uint16_t nuevo_voltaje = 3850; /* 0x0F0A en hexadecimal */
    uint8_t nuevos_flags = 0xF8;   /* Valor real: 0x8, pero trae basura 0xF arriba */

    /* TODO 3: Usa MODIFY_FIELD dos veces sobre 'registro_bms':
     * 1. Actualiza el carril de flags_error (Ancho: 4, Posición: 8) con 'nuevos_flags'.
     * 2. Actualiza el carril de voltaje_mv (Ancho: 16, Posición: 12) con 'nuevo_voltaje'.
     * Recuerda reasignar el resultado a 'registro_bms' (ej: registro_bms = MODIFY_FIELD(...);)
     */
    registro_bms = MODIFY_FIELD(registro_bms, 4, 8, nuevos_flags);
    registro_bms = MODIFY_FIELD(registro_bms, 16, 12, nuevo_voltaje);

    printf("[FINAL]  Registro actualizado     : 0x%08X\n", registro_bms);

    /* El Tribunal verifica que los valores viejos no se hayan mezclado con los nuevos */
    assert(UNPACK_FIELD(registro_bms, 0, 8) == 3);      /* El ID #3 nunca se tocó */
    assert(UNPACK_FIELD(registro_bms, 8, 4) == 0x08);   /* Pasó de 0x09 a 0x08 limpio */
    assert(UNPACK_FIELD(registro_bms, 12, 16) == 3850); /* Pasó de 4250 a 3850 mV */
    assert(registro_bms == 0x00F0A803U);

    printf("\n✅ Día 6 superado: Modificación Read-Modify-Write completada sin bits fantasma.\n");
    return 0;
}