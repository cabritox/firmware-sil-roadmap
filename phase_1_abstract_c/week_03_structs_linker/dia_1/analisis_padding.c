#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

/* 1. Desalineada con padding interno y final */
typedef struct
{
    uint8_t id;          /* 1 byte */
    uint32_t timestamp;  /* 4 bytes */
    uint16_t tension_mv; /* 2 bytes */
    uint8_t flags;       /* 1 byte */
} SensorDesalineado_t;

/* 2. Empaquetada forzada (sin padding) */
typedef struct __attribute__((packed))
{
    uint8_t id;          /* 1 byte */
    uint32_t timestamp;  /* 4 bytes */
    uint16_t tension_mv; /* 2 bytes */
    uint8_t flags;       /* 1 byte */
} SensorEmpaquetado_t;

/* 3. Optimizada manualmente por orden de tamaño */
typedef struct
{
    uint32_t timestamp;  /* 4 bytes */
    uint16_t tension_mv; /* 2 bytes */
    uint8_t id;          /* 1 byte */
    uint8_t flags;       /* 1 byte */
} SensorOptimo_t;

static void dump_memoria(const char *etiqueta, const void *ptr, size_t size)
{
    const uint8_t *b = (const uint8_t *)ptr;
    printf("%-24s [%2zu bytes]: ", etiqueta, size);
    for (size_t i = 0; i < size; i++)
    {
        printf("%02X ", b[i]);
    }
    printf("\n");
}

int main(void)
{
    SensorDesalineado_t s_pad = {
        .id = 0xAA,
        .timestamp = 0x12345678,
        .tension_mv = 0xBEEF,
        .flags = 0x55};

    SensorEmpaquetado_t s_pack = {
        .id = 0xAA,
        .timestamp = 0x12345678,
        .tension_mv = 0xBEEF,
        .flags = 0x55};

    SensorOptimo_t s_opt = {
        .timestamp = 0x12345678,
        .tension_mv = 0xBEEF,
        .id = 0xAA,
        .flags = 0x55};

    printf("============================================================\n");
    printf("   VOLCADO HEXADECIMAL DE ESTRUCTURAS EN MEMORIA RAM        \n");
    printf("============================================================\n");

    dump_memoria("1. Desalineada (Padding)", &s_pad, sizeof(s_pad));
    dump_memoria("2. Empaquetada (Packed)", &s_pack, sizeof(s_pack));
    dump_memoria("3. Optimizada  (Reorden)", &s_opt, sizeof(s_opt));

    return 0;
}