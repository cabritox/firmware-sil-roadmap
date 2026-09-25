#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/* =========================================================================
 * 1. CAPA HAL: Extracción y Bit Testing (Día 6)
 * ========================================================================= */
#define BIT_MASK(ANCHO) (((ANCHO) >= 32U) ? 0xFFFFFFFFUL : ((1UL << (ANCHO)) - 1UL))
#define UNPACK_FIELD(TRAMA, POSICION, ANCHO) (((TRAMA) >> (POSICION)) & BIT_MASK(ANCHO))

/* TODO 1: Define las dos macros de auditoría (Bit Testing)
 * - TEST_ANY_MASK(REG, MASK): Evalúa a verdadero (1) si AL MENOS UN bit de MASK está encendido en REG.
 * - TEST_ALL_MASK(REG, MASK): Evalúa a verdadero (1) si TODOS los bits de MASK están encendidos en REG.
 */
#define TEST_ANY_MASK(REG, MASK) ( ((REG) & (MASK)) != 0U )
#define TEST_ALL_MASK(REG, MASK) ( ((REG) & (MASK)) == (MASK) )

/* =========================================================================
 * 2. DEFINICIÓN DE MÁSCARAS DE AUDITORÍA
 * ========================================================================= */
/* Bits [28..31] de la trama de 32 bits: Deben ser siempre 0. Si hay un 1, hubo colisión. */
#define MASCARA_COLISION_BUS 0xF0000000UL

/* Posiciones dentro del campo 'flags_error' (4 bits) */
#define FLAG_SOBREVOLTAJE (1U << 0)     /* 0x01 (0001) - Advertencia */
#define FLAG_SOBRETEMPERATURA (1U << 1) /* 0x02 (0010) - CRÍTICO     */
#define FLAG_CORTOCIRCUITO (1U << 2)    /* 0x04 (0100) - CRÍTICO     */
#define FLAG_DESBALANCE (1U << 3)       /* 0x08 (1000) - Leve        */

/* TODO 2: Crea una máscara compuesta llamada MASCARA_FLAGS_CRITICOS que una (con |)
 * únicamente FLAG_SOBRETEMPERATURA y FLAG_CORTOCIRCUITO.
 */
#define MASCARA_FLAGS_CRITICOS (FLAG_SOBRETEMPERATURA | FLAG_CORTOCIRCUITO)

/* Códigos de respuesta del Auditor */
typedef enum
{
    TRAMA_OK = 0,
    TRAMA_COLISION_BUS,
    TRAMA_ERROR_CRITICO,
    TRAMA_ERROR_CATASTROFICO
} EstadoTrama_t;

/* =========================================================================
 * 3. FUNCIÓN AUDITORA DE TELEMETRÍA
 * ========================================================================= */
EstadoTrama_t procesar_trama_segura(uint32_t trama_rx, uint16_t *out_voltaje_seguro)
{
    /* Defensa Activa sobre el puntero */
    assert(out_voltaje_seguro != NULL);

    /* TODO 3: Implementa las 3 aduanas de validación en este orden exacto:
     *
     * ADUANA A: Usa TEST_ANY_MASK con 'trama_rx' y 'MASCARA_COLISION_BUS'.
     *           Si detecta algún bit encendido en la zona reservada [28..31],
     *           retorna inmediatamente TRAMA_COLISION_BUS (sin tocar *out_voltaje_seguro).
     *
     * ADUANA B: Extrae el campo 'flags' (Posición 8, Ancho 4) usando UNPACK_FIELD.
     *           1. Primero verifica con TEST_ALL_MASK si AMBOS flags críticos están activos
     *              al mismo tiempo usando MASCARA_FLAGS_CRITICOS. Si es así, retorna TRAMA_ERROR_CATASTROFICO.
     *           2. Luego verifica con TEST_ANY_MASK si AL MENOS UNO de los flags críticos
     *              está activo. Si es así, retorna TRAMA_ERROR_CRITICO.
     *
     * ADUANA C: Si la trama superó todas las aduanas, extrae el voltaje (Posición 12, Ancho 16)
     *           con UNPACK_FIELD, guárdalo en *out_voltaje_seguro y retorna TRAMA_OK.
     */

    if(TEST_ANY_MASK(trama_rx, MASCARA_COLISION_BUS)){
        return TRAMA_COLISION_BUS;
    }

    uint8_t flags = (uint8_t)UNPACK_FIELD(trama_rx, 8, 4);

    if(TEST_ALL_MASK(flags, MASCARA_FLAGS_CRITICOS)){
        return TRAMA_ERROR_CATASTROFICO;
    }

    if(TEST_ANY_MASK(flags, MASCARA_FLAGS_CRITICOS)){
        return TRAMA_ERROR_CRITICO;
    }

    *out_voltaje_seguro = (uint16_t)UNPACK_FIELD(trama_rx, 12, 16);


    return TRAMA_OK;
}

/* =========================================================================
 * 4. BANCO DE PRUEBAS AUTOMATIZADO (ASSERT)
 * ========================================================================= */
int main(void)
{
    uint16_t voltaje_leido = 0;
    EstadoTrama_t estado;

    /* CASO 1: Trama válida (Voltaje: 4000 mV [0x0FA0], Flags: 0x08 [Solo desbalance leve]) */
    uint32_t trama_limpia = 0x00FA0803UL;
    estado = procesar_trama_segura(trama_limpia, &voltaje_leido);
    printf("[CASO 1] Trama Limpia       -> Estado: %d | Voltaje procesado: %u mV\n", estado, voltaje_leido);
    assert(estado == TRAMA_OK);
    assert(voltaje_leido == 4000);

    /* CASO 2: Colisión en el cable (Bit 30 encendido por ruido: 0x40FA0803) */
    voltaje_leido = 0; /* Reiniciamos para comprobar que el auditor NO escriba aquí */
    uint32_t trama_colision = 0x40FA0803UL;
    estado = procesar_trama_segura(trama_colision, &voltaje_leido);
    printf("[CASO 2] Colisión en Bus    -> Estado: %d | Voltaje procesado: %u mV (Bloqueado)\n", estado, voltaje_leido);
    assert(estado == TRAMA_COLISION_BUS);
    assert(voltaje_leido == 0); /* La carga útil jamás debió procesarse */

    /* CASO 3: Un flag crítico encendido (Sobretemperatura: 0x2 en flags) */
    uint32_t trama_critica = 0x0109A203UL;
    estado = procesar_trama_segura(trama_critica, &voltaje_leido);
    printf("[CASO 3] Error Crítico      -> Estado: %d | Voltaje procesado: %u mV (Bloqueado)\n", estado, voltaje_leido);
    assert(estado == TRAMA_ERROR_CRITICO);
    assert(voltaje_leido == 0);

    /* CASO 4: Fallo Catastrófico (Sobretemperatura + Cortocircuito + Sobrevoltaje = 0x7 en flags) */
    uint32_t trama_catastrofica = 0x0109A703UL;
    estado = procesar_trama_segura(trama_catastrofica, &voltaje_leido);
    printf("[CASO 4] Error Catastrófico -> Estado: %d | Voltaje procesado: %u mV (Bloqueado)\n", estado, voltaje_leido);
    assert(estado == TRAMA_ERROR_CATASTROFICO);
    assert(voltaje_leido == 0);

    printf("\n✅ Día 6 superado: Auditoría de colisiones y Bit Testing validados con assert.\n");
    return 0;
}