#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/* =========================================================================
 * 1. CAPA HAL: Macros de Control y Extracción (Días 3 y 5)
 * ========================================================================= */
#define SET_BIT(REG, BIT) ((REG) |= (1U << (BIT)))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(1U << (BIT)))
#define READ_BIT(REG, BIT) (((REG) >> (BIT)) & 1U)
#define BIT_MASK(ANCHO) ((1U << ANCHO) - 1U)
#define PACK_FIELD(VALOR, ANCHO, POSICION) (((uint32_t)(VALOR) & BIT_MASK(ANCHO)) << (POSICION))
#define UNPACK_FIELD(TRAMA, POSICION, ANCHO) (((TRAMA) >> (POSICION)) & BIT_MASK(ANCHO))

/* Definición del Mapa de Bits para 'flags_error' (4 bits disponibles: 0..3) */
#define FLAG_SOBREVOLTAJE 0     /* Bit 0: Voltaje crítico en la celda */
#define FLAG_SOBRETEMPERATURA 1 /* Bit 1: Temperatura fuera de rango */
#define FLAG_CORTOCIRCUITO 2    /* Bit 2: Corriente de descarga excesiva */
#define FLAG_DESBALANCE 3       /* Bit 3: Celda desbalanceada respecto al pack */

/* =========================================================================
 * 2. NODO TRANSMISOR (Día 4)
 * ========================================================================= */
uint32_t empaquetar_trama(uint8_t id_celda, uint8_t flags_error, uint16_t voltaje_mv)
{
    uint32_t trama = 0;

    trama |=(uint32_t)PACK_FIELD(id_celda,8,0);
    trama |=(uint32_t)PACK_FIELD(flags_error,4,8);
    trama |=(uint32_t)PACK_FIELD(voltaje_mv,16,12);

    return trama;

}

/* =========================================================================
 * 3. NODO RECEPTOR (Día 5)
 * ========================================================================= */
void desempaquetar_trama(uint32_t trama_rx, uint8_t *out_id, uint8_t *out_flags, uint16_t *out_voltaje)
{
    /* TODO 2: Pega aquí tu lógica blindada de desempaquetar_trama() del Día 5
     * (Incluyendo los 3 asserts de punteros NULL y el downcasting explícito)
     */
    assert(out_id != NULL);
    assert(out_flags != NULL);
    assert(out_voltaje != NULL);

    *out_id = (uint8_t)UNPACK_FIELD(trama_rx, 0, 8);
    *out_flags = (uint8_t)UNPACK_FIELD(trama_rx, 8, 4);
    *out_voltaje = (uint16_t)UNPACK_FIELD(trama_rx, 12, 16);

}

/* =========================================================================
 * 4. SIMULACIÓN DE BUCLE CERRADO (TX -> CABLE RUIDOSO -> RX)
 * ========================================================================= */
int main(void)
{
    /* --- ETAPA A: Lectura en el Sensor de la Celda #3 --- */
    uint8_t tx_id = 3;
    uint16_t tx_voltaje = 4250; /* ¡4.25V! Celda sobrecargada */
    uint8_t tx_flags = 0;       /* Inicia sin alertas */

    /* TODO 3: Usa tu macro SET_BIT sobre 'tx_flags' para encender ÚNICAMENTE
     * las alertas FLAG_SOBREVOLTAJE (Bit 0) y FLAG_DESBALANCE (Bit 3).
     * Además, inyectaremos basura a propósito en el Bit 7 para poner a prueba tu sanitizador:
     */

    /* <-- Escribe tus dos llamadas a SET_BIT aquí */
    SET_BIT(tx_flags, FLAG_SOBREVOLTAJE);
    SET_BIT(tx_flags, FLAG_DESBALANCE);
    SET_BIT(tx_flags, 7); /* Ruido interno en el byte de flags */

    /* Empaquetamos para enviar */
    uint32_t trama_tx = empaquetar_trama(tx_id, tx_flags, tx_voltaje);
    printf("[TX] Trama enviada al bus       : 0x%08X\n", trama_tx);

    /* --- ETAPA B: El Cable Físico (Inyección de Interferencia) --- */
    /* Ensuciamos los bits reservados [28..31] con un 0xE en el camino */
    uint32_t cable_can_rx = trama_tx | 0xE0000000U;
    printf("[BUS] Trama en cable (con ruido): 0x%08X\n", cable_can_rx);

    /* --- ETAPA C: Recepción en el Microcontrolador Maestro --- */
    uint8_t rx_id = 0;
    uint8_t rx_flags = 0;
    uint16_t rx_voltaje = 0;

    desempaquetar_trama(cable_can_rx, &rx_id, &rx_flags, &rx_voltaje);

    printf("[RX] Datos recuperados          : Celda #%u | Voltaje: %u mV | Flags: 0x%02X\n",
           rx_id, rx_voltaje, rx_flags);

    /* --- ETAPA D: Toma de Decisiones y Auditoría (Asserts) --- */
    uint8_t mosfet_corte_activo = 0;

    /* TODO 4: Escribe un condicional 'if' usando tu macro READ_BIT sobre 'rx_flags'.
     * Si el bit FLAG_SOBREVOLTAJE está en 1, asigna mosfet_corte_activo = 1.
     */
    if(READ_BIT(rx_flags, FLAG_SOBREVOLTAJE)){
        mosfet_corte_activo = 1;
    }

    /* El Tribunal verifica la integridad de extremo a extremo */
    assert(rx_id == 3);
    assert(rx_voltaje == 4250);
    assert(rx_flags == 0x09); /* Bit 0 (1) + Bit 3 (8) = 9. El Bit 7 de basura debió morir */
    assert(READ_BIT(rx_flags, FLAG_SOBREVOLTAJE) == 1);
    assert(READ_BIT(rx_flags, FLAG_DESBALANCE) == 1);
    assert(READ_BIT(rx_flags, FLAG_SOBRETEMPERATURA) == 0);
    assert(mosfet_corte_activo == 1);

    printf("\n🚨 ALERTA BMS: Sobrevoltaje detectado en Celda #%u (%u mV). MOSFET de corte ACTIVADO.\n",
           rx_id, rx_voltaje);
    printf("✅ Prueba de Loopback superada sin pérdida ni corrupción de datos.\n");

    return 0;
}