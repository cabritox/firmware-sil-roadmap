#include "buffer.h"

void init_buffer(RingBuffer *buf){
    if (buf == NULL){
        return;
    }
    buf->head = 0;
    buf->tail=0;
    buf->count=0;
    return;
};

EstadoBuffer buffer_push(RingBuffer *buf, uint8_t valor){
    if (buf == NULL){
        return ERROR_PUNTERO_NULO;
    }

    if (buf->count == BUFFER_SIZE){
        return BUFFER_LLENO;
    }

    buf->datos[buf->head] = valor;
    buf->head = (buf->head + 1) % BUFFER_SIZE;
    buf->count+=1;
    return BUFFER_OK;
}

EstadoBuffer buffer_pop(RingBuffer *buf, uint8_t *valor_leido){
    if (buf == NULL || valor_leido == NULL){
        return ERROR_PUNTERO_NULO;
    }

    if (buf->count == 0){
        return BUFFER_VACIO;
    }

    *valor_leido = *(buf->datos + buf->tail);
    buf->tail = (buf->tail + 1) % BUFFER_SIZE;
    buf->count--;
    return BUFFER_OK;
}


