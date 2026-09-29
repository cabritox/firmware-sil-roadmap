#ifndef BMS_PROTOCOL_H
#define BMS_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

#define BIT_MASK(ANCHO) (((ANCHO) >= 32) ? 0xFFFFFFFFUL : ((1UL << (ANCHO)) - 1UL))
#define PACK_FIELD(VALOR, ANCHO, POSICION) (((uint32_t)(VALOR) & BIT_MASK(ANCHO)) << (POSICION))
#define UNPACK_FIELD(TRAMA, POSICION, ANCHO) (((TRAMA) >> (POSICION)) & BIT_MASK(ANCHO))
#define CLEAR_FIELD(REG, ANCHO, POSICION) (((REG) & ~((uint32_t)BIT_MASK(ANCHO) << (POSICION))))
#define MODIFY_FIELD(REG, ANCHO, POSICION, VALOR) ((CLEAR_FIELD(REG, ANCHO, POSICION)) | (PACK_FIELD(VALOR, ANCHO, POSICION)))

#define TEST_ANY_MASK(REG, MASK) ( ((REG) & (MASK)) != 0U )
#define TEST_ALL_MASK(REG, MASK) ( ((REG) & (MASK)) == (MASK) )

/*
    MAPA DE TRAMAS BMS GUARD DE 32 BITS
*/
/*Carril ID celda*/
#define BMS_ID_POS 0
#define BMS_ID_WIDTH 4
    
/*Carril milivoltios*/
#define BMS_VOLT_POS 4 
#define BMS_VOLT_WIDTH 12

/*Carril temperatura*/
#define BMS_TEMP_POS 16
#define BMS_TEMP_WIDTH 8

/*Carril distancia*/
#define BMS_DIST_POS 24
#define BMS_DIST_WIDTH 4

/*Carril flags criticos*/
#define BMS_FLAGS_POS 28
#define BMS_FLAGS_WIDTH 4

/*Mascaras de bit alarmas criticas*/
#define BMS_FLAG_SOBRETENSION (1UL << 28)
#define BMS_FLAG_GABINETE_ABIERTO (1UL << 29)
#define BMS_FLAG_ALERTA_TERMICA (1UL << 30)
#define BMS_FLAG_FUEGO_IGNICION (1UL << 31)

/*Mascara global para auditar en 0(1) para cualquier alarma critica activa*/
#define BMS_MASCARA_CRITICA (0xF0000000UL)

/*Constante capacidad fija de cola*/
#define BMS_QUEUE_CAPACITY 8U

typedef enum {
    BMS_OK = 0,
    BMS_ERR_NULL,
    BMS_ERR_VACIO,
    BMS_ERR_LLENO,
    BMS_ALARMA_CRITICA 
} BmsStatus_t;

typedef struct {
    uint16_t milivoltios;
    uint8_t id_celda;
    uint8_t temperatura;
    uint8_t distancia_tapa;
    uint8_t flags_criticos;
} BmsTelemetry_t;

typedef struct {
    uint32_t buffer[BMS_QUEUE_CAPACITY];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
}RingBuffer32_t;

/*Inicializar cola*/
BmsStatus_t bms_init_queue(RingBuffer32_t *rb);
/*Empaquetar y enconlar una trama*/
BmsStatus_t bms_enqueue_telemetry(RingBuffer32_t *rb, const BmsTelemetry_t *telemetria);
/*Modificar un carril en caliente (READ_MODIFY_WRITE) */
BmsStatus_t bms_update_field(uint32_t *trama, uint8_t ancho, uint8_t posicion, uint32_t nuevo_valor);
/*Desencolar y procesar trama segura*/
BmsStatus_t bms_dequeue_and_process(RingBuffer32_t *rb, BmsTelemetry_t *out_telemetria);



#endif