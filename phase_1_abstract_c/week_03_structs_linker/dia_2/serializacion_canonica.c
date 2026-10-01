#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/* =========================================================================
   SERIALIZADORES BIG-ENDIAN (Network Byte Order)
   El byte mas significativo (MSB) se coloca en el indice mas bajo.
   ========================================================================= */

void ser_write_u16_be(uint8_t *buf, uint16_t val)
{
    buf[0] = (uint8_t)((val >> 8) & 0xFF);
    buf[1] = (uint8_t)(val & 0xFF);
}

uint16_t ser_read_u16_be(const uint8_t *buf)
{
    return (uint16_t)(((uint16_t)buf[0] << 8) |
                      ((uint16_t)buf[1]));
}

void ser_write_u32_be(uint8_t *buf, uint32_t val)
{
    buf[0] = (uint8_t)((val >> 24) & 0xFF);
    buf[1] = (uint8_t)((val >> 16) & 0xFF);
    buf[2] = (uint8_t)((val >> 8) & 0xFF);
    buf[3] = (uint8_t)(val & 0xFF);
}

uint32_t ser_read_u32_be(const uint8_t *buf)
{
    /* Casteo explicito a uint32_t indispensable para microcontroladores de 8 bits */
    return (((uint32_t)buf[0] << 24) |
            ((uint32_t)buf[1] << 16) |
            ((uint32_t)buf[2] << 8) |
            ((uint32_t)buf[3]));
}

/* =========================================================================
   SERIALIZADORES LITTLE-ENDIAN
   El byte menos significativo (LSB) se coloca en el indice mas bajo.
   ========================================================================= */

void ser_write_u32_le(uint8_t *buf, uint32_t val)
{
    buf[0] = (uint8_t)(val & 0xFF);
    buf[1] = (uint8_t)((val >> 8) & 0xFF);
    buf[2] = (uint8_t)((val >> 16) & 0xFF);
    buf[3] = (uint8_t)((val >> 24) & 0xFF);
}

uint32_t ser_read_u32_le(const uint8_t *buf)
{
    return (((uint32_t)buf[0]) |
            ((uint32_t)buf[1] << 8) |
            ((uint32_t)buf[2] << 16) |
            ((uint32_t)buf[3] << 24));
}

/* =========================================================================
   BANCO DE PRUEBAS Y DEMOSTRACION
   ========================================================================= */

static void print_buffer_hex(const char *label, const uint8_t *buf, size_t len)
{
    printf("%-26s: ", label);
    for (size_t i = 0; i < len; i++)
    {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

int main(void)
{
    uint8_t trama[10] = {0};
    const uint32_t VALOR_32 = 0x12345678;
    const uint16_t VALOR_16 = 0xABCD;

    printf("====================================================\n");
    printf("   DEMOSTRACION: SERIALIZACION CANONICA EN C        \n");
    printf("====================================================\n\n");

    /* 1. Demostracion Big-Endian vs Little-Endian */
    uint8_t buf_be[4];
    uint8_t buf_le[4];

    ser_write_u32_be(buf_be, VALOR_32);
    ser_write_u32_le(buf_le, VALOR_32);

    print_buffer_hex("Big-Endian (Humano/Red)", buf_be, 4);
    print_buffer_hex("Little-Endian (x86/AVR)", buf_le, 4);

    /* 2. Prueba de acceso desalineado seguro (Posicion impar) */
    /* Escribimos un u32 en trama[1] y un u16 en trama[5] */
    ser_write_u32_be(&trama[1], VALOR_32);
    ser_write_u16_be(&trama[5], VALOR_16);

    print_buffer_hex("Trama desalineada [0..7]", trama, 8);

    /* 3. Reconstruccion y Verificacion con Aserciones */
    uint32_t valor_leido_32 = ser_read_u32_be(&trama[1]);
    uint16_t valor_leido_16 = ser_read_u16_be(&trama[5]);

    assert(valor_leido_32 == VALOR_32);
    assert(valor_leido_16 == VALOR_16);

    /* 4. Verificacion de Roundtrip con Little-Endian */
    assert(ser_read_u32_le(buf_le) == VALOR_32);

    printf("\n[PASS] Todas las pruebas de serializacion pasaron con exito.\n");
    return 0;
}