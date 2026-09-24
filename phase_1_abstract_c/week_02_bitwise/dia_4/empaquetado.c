#include <stdio.h>
#include <stdint.h>
#include <assert.h>

uint32_t empaquetar_trama(uint8_t id_celda, uint8_t flags_error, uint16_t voltaje_mv)
{

    /* TODO:
     * 1. Prepara cada variable (Castea a uint32_t, sanitiza con su máscara & y desplázala con <<)
     * 2. Únelas todas dentro de 'trama' usando el operador OR (|)
     */
    uint8_t id_limpio = id_celda & 0xFFU;
    uint8_t flags_limpio = flags_error & 0x0FU;
    uint16_t voltaje_limpio = voltaje_mv & 0xFFFFU;

    uint32_t id_en_carril = (uint32_t)id_limpio;
    uint32_t flags_en_carril = ((uint32_t)flags_limpio) << 8;
    uint32_t voltaje_en_carril = ((uint32_t)voltaje_limpio) << 12;
    
    uint32_t trama = voltaje_en_carril | flags_en_carril | id_en_carril;

    return trama;
}

int main(void)
{
    /* PRUEBA 1: Empaquetado estándar con datos limpios */
    uint8_t id = 0x4A;         /* Carril: 0..7   (2 dígitos hex) */
    uint8_t flags = 0x0B;      /* Carril: 8..11  (1 dígito hex)  */
    uint16_t voltaje = 0x1234; /* Carril: 12..27 (4 dígitos hex) */

    uint32_t trama_1 = empaquetar_trama(id, flags, voltaje);
    printf("Trama 1 (Limpia)     : 0x%08X\n", trama_1);
    assert(trama_1 == 0x01234B4A);

    /* PRUEBA 2: Ataque de Contaminación de Carril (Bleed-over) */
    uint8_t flags_corruptos = 0xFB;
    uint32_t trama_2 = empaquetar_trama(id, flags_corruptos, voltaje);
    printf("Trama 2 (Sanitizada) : 0x%08X\n", trama_2);
    assert(trama_2 == 0x01234B4A);

    printf("\n✅ Día 4 superado: Trama de 32 bits empaquetada y blindada.\n");
    return 0;
}