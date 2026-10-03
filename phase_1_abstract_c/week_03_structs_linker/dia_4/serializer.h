#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <stdint.h>
#include <stddef.h>

/*
 * Tamano exacto de la trama en el cable (sin padding):
 * timestamp (4) + voltaje (2) + corriente (2) + temperatura (1) + flags (1) = 10 bytes
 */
#define BMS_FRAME_SIZE 10

typedef enum
{
    SERIALIZER_OK = 0,
    SERIALIZER_ERR_NULL = 1,
    SERIALIZER_ERR_BUFFER_SIZE = 2 /* El buffer de destino es muy pequeno */
} SerializerStatus_t;

/* Estructura optimizada para la CPU (puede tener padding interno) */
typedef struct
{
    uint32_t timestamp_ms; /* 4 bytes */
    uint16_t voltaje_mv;   /* 2 bytes */
    uint16_t corriente_ma; /* 2 bytes */
    uint8_t temperatura;   /* 1 byte  */
    uint8_t flags_estado;  /* 1 byte  */
} BmsFisico_t;

/* =========================================================================
   PROTOTIPOS DEL SERIALIZADOR
   ========================================================================= */

/**
 * Empaqueta la estructura a un arreglo de bytes en formato Big-Endian.
 * @param in_data Puntero a la estructura de origen.
 * @param out_buffer Puntero al arreglo de destino.
 * @param buffer_size Tamano del arreglo (debe ser >= BMS_FRAME_SIZE).
 */
SerializerStatus_t bms_serialize_data(const BmsFisico_t *in_data,
                                        uint8_t *out_buffer,
                                        size_t buffer_size);

/**
 * Desempaqueta un arreglo de bytes Big-Endian hacia la estructura local.
 * @param in_buffer Puntero al arreglo de origen.
 * @param buffer_size Tamano del arreglo (debe ser >= BMS_FRAME_SIZE).
 * @param out_data Puntero a la estructura de destino.
 */
SerializerStatus_t bms_deserialize_data(const uint8_t *in_buffer,
                                        size_t buffer_size,
                                        BmsFisico_t *out_data);

#endif /* SERIALIZER_H */