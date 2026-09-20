#ifndef BUFFER_H
#define BUFFER_H
#include <stdint.h>
#include <stddef.h>

#define BUFFER_SIZE 5

typedef enum {
    BUFFER_OK,
    BUFFER_LLENO,
    BUFFER_VACIO,
    ERROR_PUNTERO_NULO
}EstadoBuffer;

typedef struct {
    uint8_t datos[BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
}RingBuffer;

void init_buffer(RingBuffer *buf);
EstadoBuffer buffer_push(RingBuffer *buf, uint8_t valor);
EstadoBuffer buffer_pop(RingBuffer *buf, uint8_t *valor_leido);

#endif