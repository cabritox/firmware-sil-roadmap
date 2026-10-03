#include <stdio.h>
#include <assert.h>
#include "serializer.h"

int main(void)
{
    /* 1. Datos originales simulados del Arduino */
    BmsFisico_t bms_original = {
        .timestamp_ms = 0xAABBCCDD, /* 2864434397 ms */
        .voltaje_mv = 4200,         /* 4.2V */
        .corriente_ma = 1500,       /* 1.5A */
        .temperatura = 35,          /* 35°C */
        .flags_estado = 0x01        /* Celda balanceando */
    };

    uint8_t buffer_tx[BMS_FRAME_SIZE] = {0};
    BmsFisico_t bms_recuperado = {0};

    /* 2. Empaquetar a bytes */
    SerializerStatus_t st_tx = bms_serialize_data(&bms_original, buffer_tx, sizeof(buffer_tx));
    assert(st_tx == SERIALIZER_OK);

    /* 3. Desempaquetar desde bytes */
    SerializerStatus_t st_rx = bms_deserialize_data(buffer_tx, sizeof(buffer_tx), &bms_recuperado);
    assert(st_rx == SERIALIZER_OK);

    /* 4. Comprobar que no se perdio ni un solo bit en el viaje */
    assert(bms_original.timestamp_ms == bms_recuperado.timestamp_ms);
    assert(bms_original.voltaje_mv == bms_recuperado.voltaje_mv);
    assert(bms_original.corriente_ma == bms_recuperado.corriente_ma);
    assert(bms_original.temperatura == bms_recuperado.temperatura);
    assert(bms_original.flags_estado == bms_recuperado.flags_estado);

    printf("[EXITO] La serializacion bidireccional es 100%% precisa.\n");
    return 0;
}