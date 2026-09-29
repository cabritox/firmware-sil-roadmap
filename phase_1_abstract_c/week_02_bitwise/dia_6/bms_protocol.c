#include "bms_protocol.h"
#include <stddef.h>

BmsStatus_t bms_init_queue(RingBuffer32_t *rb){
    if (rb == NULL){
        return BMS_ERR_NULL;
    }
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    return BMS_OK;
}

BmsStatus_t bms_enqueue_telemetry(RingBuffer32_t *rb, const BmsTelemetry_t *telemetria){
    if(rb == NULL || telemetria == NULL){
        return BMS_ERR_NULL;
    }

    if ((rb->count == BMS_QUEUE_CAPACITY)){
        return BMS_ERR_LLENO;
    }
    
    uint32_t trama = 0;

    trama |= (uint32_t)PACK_FIELD(telemetria->id_celda,BMS_ID_WIDTH,BMS_ID_POS);
    trama |= (uint32_t)PACK_FIELD(telemetria->milivoltios, BMS_VOLT_WIDTH,BMS_VOLT_POS);
    trama |= (uint32_t)PACK_FIELD(telemetria->temperatura,BMS_TEMP_WIDTH,BMS_TEMP_POS);
    trama |= (uint32_t)PACK_FIELD(telemetria->distancia_tapa, BMS_DIST_WIDTH, BMS_DIST_POS);
    trama |= (uint32_t)PACK_FIELD(telemetria->flags_criticos, BMS_FLAGS_WIDTH,BMS_FLAGS_POS);

    rb->buffer[rb->head]=trama;
    rb->head = (rb->head + 1) % BMS_QUEUE_CAPACITY;
    rb->count++;
    return BMS_OK;
}

BmsStatus_t bms_update_field(uint32_t *trama, uint8_t ancho, uint8_t posicion, uint32_t nuevo_valor){
    if(trama == NULL){
        return BMS_ERR_NULL;
    }

    *trama = (uint32_t) MODIFY_FIELD(*trama, ancho, posicion, nuevo_valor);

    return BMS_OK;
}

static bool tiene_alarma_critica(uint32_t trama){
    return TEST_ANY_MASK(trama, BMS_MASCARA_CRITICA);
}

BmsStatus_t bms_dequeue_and_process(RingBuffer32_t *rb, BmsTelemetry_t *out_telemetria){
    if (rb == NULL || out_telemetria == NULL){
        return BMS_ERR_NULL;
    }

    if (rb->count == 0){
        return BMS_ERR_VACIO;
    }

    uint32_t trama = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % BMS_QUEUE_CAPACITY;
    rb->count--;

    if(tiene_alarma_critica(trama)){
        return BMS_ALARMA_CRITICA;
    }

    out_telemetria->milivoltios = (uint16_t)UNPACK_FIELD(trama, BMS_VOLT_POS, BMS_VOLT_WIDTH);
    out_telemetria->id_celda = (uint8_t)UNPACK_FIELD(trama, BMS_ID_POS, BMS_ID_WIDTH);
    out_telemetria->temperatura = (uint8_t)UNPACK_FIELD(trama, BMS_TEMP_POS, BMS_TEMP_WIDTH);
    out_telemetria->distancia_tapa = (uint8_t)UNPACK_FIELD(trama, BMS_DIST_POS, BMS_DIST_WIDTH);
    out_telemetria->flags_criticos = (uint8_t)UNPACK_FIELD(trama, BMS_FLAGS_POS, BMS_FLAGS_WIDTH);

    return BMS_OK;
}


