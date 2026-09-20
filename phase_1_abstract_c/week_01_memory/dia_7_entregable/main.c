#include <stdio.h>
#include <assert.h>
#include "buffer.h"

uint8_t variable_global = 10;

int main (void){
    RingBuffer mi_buffer;
    
    printf("Direccion en memoria variable global: %p", &variable_global);
    printf("\nDireccion en memoria mi buffer: %p\n", &mi_buffer);

    init_buffer(&mi_buffer);

    assert(buffer_push(&mi_buffer, 100) == BUFFER_OK);
    assert(buffer_push(&mi_buffer, 50) == BUFFER_OK);
    assert(buffer_push(&mi_buffer, 25) == BUFFER_OK);
    assert(buffer_push(&mi_buffer, 75) == BUFFER_OK);
    assert(buffer_push(&mi_buffer, 200) == BUFFER_OK);

    assert(buffer_push(&mi_buffer, 255) == BUFFER_LLENO);

    uint8_t dato_leido;

    assert(buffer_pop(&mi_buffer, &dato_leido) == BUFFER_OK); 
    assert(buffer_pop(&mi_buffer, &dato_leido) == BUFFER_OK); 
    assert(buffer_pop(&mi_buffer, &dato_leido) == BUFFER_OK); 
    assert(buffer_pop(&mi_buffer, &dato_leido) == BUFFER_OK); 
    assert(buffer_pop(&mi_buffer, &dato_leido) == BUFFER_OK);

    assert(buffer_pop(&mi_buffer, &dato_leido) == BUFFER_VACIO);

    printf("\n Entregable Semana 1: Búfer SPSC validado exitosamente.\n");
}