#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/* TODO 1: Define la macro industrial UNPACK_FIELD(TRAMA, POSICION, MASCARA)
 * Recuerda proteger cada parámetro y toda la expresión con paréntesis.
 */
#define UNPACK_FIELD(TRAMA, POSICION, MASCARA) ( ((TRAMA) >> (POSICION)) & (MASCARA))/* <-- Escribe tu macro aquí */

/*
 * Función receptora: Extrae los 3 campos de 'trama_rx' y los guarda
 * directamente en las direcciones de memoria apuntadas por los punteros.
 */
void desempaquetar_trama(uint32_t trama_rx, uint8_t *out_id, uint8_t *out_flags, uint16_t *out_voltaje)
{
    /* Defensa Activa (Semana 1): Prohibido operar si algún puntero es NULL */
    assert(out_id != NULL);
    assert(out_flags != NULL);
    assert(out_voltaje != NULL);

    /* TODO 2: Usa tu macro UNPACK_FIELD para extraer cada carril y
     * asignarlo mediante desreferenciación (*out_...) a su variable.
     *
     * Recordatorio del Mapa de Trama:
     * - id_celda    : Posición 0,  Máscara de 8 bits  (0xFFU)
     * - flags_error : Posición 8,  Máscara de 4 bits  (0x0FU)
     * - voltaje_mv  : Posición 12, Máscara de 16 bits (0xFFFFU)
     */
    *out_id = (uint8_t)UNPACK_FIELD(trama_rx, 0, 0xFFU);
    *out_flags = (uint8_t)UNPACK_FIELD(trama_rx, 8, 0x0FU);
    *out_voltaje = (uint16_t)UNPACK_FIELD(trama_rx, 12, 0xFFFFU);
}

int main(void)
{
    /* Trama cruda que llega desde el cable CAN Bus (la misma que armaste en el Día 4) */
    uint32_t trama_recibida = 0x01234B4A;

    /* Variables vacías donde depositaremos los datos extraídos */
    uint8_t id_extraido = 0;
    uint8_t flags_extraidos = 0;
    uint16_t voltaje_extraido = 0;

    /* Pasamos las direcciones de memoria (&) para que la función las rellene */
    desempaquetar_trama(trama_recibida, &id_extraido, &flags_extraidos, &voltaje_extraido);

    printf("--- TELEMETRÍA RECIBIDA Y DESEMPAQUETADA ---\n");
    printf("ID Celda    : 0x%02X (%u)\n", id_extraido, id_extraido);
    printf("Flags Error : 0x%02X (%u)\n", flags_extraidos, flags_extraidos);
    printf("Voltaje     : 0x%04X (%u mV)\n", voltaje_extraido, voltaje_extraido);

    /* El Tribunal valida que la extracción sea quirúrgicamente exacta */
    assert(id_extraido == 0x4A);
    assert(flags_extraidos == 0x0B);
    assert(voltaje_extraido == 0x1234);

    /* PRUEBA DE FUEGO: ¿Qué pasa si los bits reservados [28..31] traen ruido eléctrico (ej. 0xF)?
     * Si tu máscara de voltaje (0xFFFFU) está bien hecha, ese ruido superior será destruido.
     */
    uint32_t trama_con_ruido_alto = 0xF1234B4A;
    desempaquetar_trama(trama_con_ruido_alto, &id_extraido, &flags_extraidos, &voltaje_extraido);
    assert(voltaje_extraido == 0x1234);

    printf("\n✅ Día 5 superado: Extracción completada y blindada contra ruido en bits reservados.\n");
    return 0;
}