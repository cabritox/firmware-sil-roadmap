#include "serializer.h"

SerializerStatus_t bms_serialize_data(const BmsFisico_t *in_data, uint8_t *out_buffer, size_t buffer_size){
    if (in_data == NULL || out_buffer == NULL){
        return SERIALIZER_ERR_NULL;
    }
    if (buffer_size < BMS_FRAME_SIZE){
        return SERIALIZER_ERR_BUFFER_SIZE;
    }

    out_buffer[0] = (uint8_t)((in_data->timestamp_ms >> 24) & ((1U << 8)-1U));
    out_buffer[1] = (uint8_t)((in_data->timestamp_ms >> 16) & ((1U << 8)-1U));
    out_buffer[2] = (uint8_t)((in_data->timestamp_ms >>  8) & ((1U << 8)-1U));
    out_buffer[3] = (uint8_t)((in_data->timestamp_ms >>  0) & ((1U << 8)-1U));

    out_buffer[4] = (uint8_t)((in_data->voltaje_mv >> 8) & ((1U << 8)-1U));
    out_buffer[5] = (uint8_t)((in_data->voltaje_mv >> 0) & ((1U << 8)-1U));

    out_buffer[6] = (uint8_t)((in_data->corriente_ma >> 8) & ((1U << 8)-1U));
    out_buffer[7] = (uint8_t)((in_data->corriente_ma >> 0) & ((1U << 8)-1U));

    out_buffer[8] = (uint8_t)((in_data->temperatura >> 0) & ((1U << 8)-1U));

    out_buffer[9] = (uint8_t)((in_data->flags_estado >> 0) & ((1U << 8)-1U));

    return SERIALIZER_OK;

}

SerializerStatus_t bms_deserialize_data(const uint8_t *in_buffer, size_t buffer_size, BmsFisico_t *out_data){
    if (in_buffer == NULL || out_data == NULL){
        return SERIALIZER_ERR_NULL;
    }
    if(buffer_size < BMS_FRAME_SIZE){
        return SERIALIZER_ERR_BUFFER_SIZE;
    }

    out_data->timestamp_ms=((uint32_t)in_buffer[0] << 24) |
                            ((uint32_t)in_buffer[1] << 16)|
                            ((uint32_t)in_buffer[2] << 8) |
                            ((uint32_t)in_buffer[3] << 0);

    out_data->voltaje_mv=((uint16_t)in_buffer[4] << 8) |
                        ((uint16_t)in_buffer[5] << 0);

    out_data->corriente_ma=((uint16_t)in_buffer[6] << 8)|
                        ((uint16_t)in_buffer[7] << 0);

    out_data->temperatura = ((uint8_t)in_buffer[8] << 0);

    out_data->flags_estado=((uint8_t)in_buffer[9] << 0);
    
    return SERIALIZER_OK;
}