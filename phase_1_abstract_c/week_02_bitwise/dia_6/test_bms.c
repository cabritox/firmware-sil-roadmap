#include <stdio.h>
#include <assert.h>
#include "bms_protocol.h"

static void test_defensa_punteros(){
    assert(bms_init_queue(NULL) == BMS_ERR_NULL);

    RingBuffer32_t rb;
    BmsTelemetry_t tel;

    assert(bms_enqueue_telemetry(NULL,&tel) == BMS_ERR_NULL);
    assert(bms_enqueue_telemetry(&rb,NULL) == BMS_ERR_NULL);

    assert(bms_dequeue_and_process(NULL,&tel) == BMS_ERR_NULL);
    assert(bms_dequeue_and_process(&rb,NULL) == BMS_ERR_NULL);
}

static void test_flujo_nominal(){
    RingBuffer32_t rb;

    assert(bms_init_queue(&rb) == BMS_OK);

    BmsTelemetry_t tx = {
        .milivoltios = 3750, 
        .id_celda = 5,       
        .temperatura = 42,   
        .distancia_tapa = 2, 
        .flags_criticos = 0
    };

    BmsTelemetry_t rx = {0};

    /*Encolar*/
    assert(bms_enqueue_telemetry(&rb,&tx) == BMS_OK);
    assert(rb.count == 1);
    /*Descencolar y Decodificar*/
    assert(bms_dequeue_and_process(&rb,&rx) == BMS_OK);
    assert(rb.count == 0);
    /*Auditoria de integridad*/
    assert(rx.milivoltios == tx.milivoltios);
    assert(rx.id_celda == tx.id_celda);
    assert(rx.temperatura == tx.temperatura);
    assert(rx.distancia_tapa == tx.distancia_tapa);
    assert(rx.flags_criticos == tx.flags_criticos);

    printf("[PASS] Flujo nominal (Roundtrip) validado con exito.\n");
}

static void test_saturacion_cola(){

    /*Verificacion en cola vacia inicial(UNDERFLOW)*/
    RingBuffer32_t rb;
    BmsTelemetry_t rx = {0};

    assert(bms_init_queue(&rb) == BMS_OK);
    assert(bms_dequeue_and_process(&rb,&rx) == BMS_ERR_VACIO);
    assert(rb.count == 0);
    /*Llenado hasta el limite nominal*/
    for (size_t i = 0; i < BMS_QUEUE_CAPACITY; i++){
        assert(bms_enqueue_telemetry(&rb,&rx) == BMS_OK);
    }
    assert(rb.count == BMS_QUEUE_CAPACITY);
    /*Intento de desbordamiento (OVERFLOW)*/
    assert(bms_enqueue_telemetry(&rb,&rx) == BMS_ERR_LLENO);
    assert(rb.count == BMS_QUEUE_CAPACITY);
    /*Vaciado total y nuevo UNDERFLOW*/
    for (size_t i = 0; i < BMS_QUEUE_CAPACITY; i++){
        assert(bms_dequeue_and_process(&rb,&rx) == BMS_OK);
    }
    assert(rb.count == 0);
    assert(bms_dequeue_and_process(&rb,&rx) == BMS_ERR_VACIO);
    printf("[PASS] Saturacion de cola (Overflow y Underflow) validada con exito.\n");
}

static void test_seguridad_e_inyeccion(){

    /*Inyeccion alarma critica*/
    RingBuffer32_t rb;
    assert(bms_init_queue(&rb) == BMS_OK);

    BmsTelemetry_t tx = {
        .milivoltios = 3600,
        .id_celda = 1,
        .temperatura = 25,
        .distancia_tapa = 10,
        .flags_criticos = 0x01 /* Encendemos el primer flag crítico */
    };

    BmsTelemetry_t rx = {0};

    assert(bms_enqueue_telemetry(&rb,&tx) == BMS_OK);
    assert(bms_dequeue_and_process(&rb,&rx) == BMS_ALARMA_CRITICA);
    assert(rb.count == 0);
    /*Modificacion de Campo*/
    uint32_t trama = 0;
    trama = PACK_FIELD(2500, BMS_VOLT_WIDTH, BMS_VOLT_POS);
    assert(bms_update_field(&trama, BMS_VOLT_WIDTH, BMS_VOLT_POS, 4000) == BMS_OK);
    assert(UNPACK_FIELD(trama, BMS_VOLT_POS, BMS_VOLT_WIDTH) == 4000);
    assert(bms_update_field(NULL, BMS_VOLT_WIDTH, BMS_VOLT_POS, 4000) == BMS_ERR_NULL);

    printf("[PASS] Seguridad e inyeccion de fallos (Alarma critica y RMW) validada con exito.\n");
}

int main(void){
    test_defensa_punteros();
    test_flujo_nominal();
    test_saturacion_cola();
    test_seguridad_e_inyeccion();
}