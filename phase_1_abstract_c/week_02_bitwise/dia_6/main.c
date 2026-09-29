#include "bms_protocol.h"
#include <avr/io.h>
#include <util/delay.h>

// Instancia global en memoria SRAM (.bss)
RingBuffer32_t cola_bms;

// -----------------------------------------------------------------------------
// 1. INICIALIZACIÓN DE PUERTOS Y DIRECCIONES
// -----------------------------------------------------------------------------
static void bms_hal_init(void)
{
    // PUERTO B: Barra de 5 LEDs
    DDRB |= 0x1F;
    PORTB &= ~0x1F;

    // PUERTO D: Relé (PD7) y Buzzer (PD6)
    DDRD |= (1 << 7) | (1 << 6);
    PORTD |= (1 << 7);  // Relé cerrado
    PORTD &= ~(1 << 6); // Buzzer apagado

    // PUERTO D: Entradas
    DDRD &= ~((1 << 2) | (1 << 3) | (1 << 4));
    PORTD |= (1 << 2); // Pull-Up en botón de rearme

    // ACTIVAR ADC: Habilitar conversor con prescaler de 128 (125 kHz)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// -----------------------------------------------------------------------------
// 2. CONVERSOR ANALÓGICO A DIGITAL (ADC)
// -----------------------------------------------------------------------------
static uint16_t bms_adc_read(uint8_t canal)
{
    // 1. Conmutar canal manteniendo referencia AVcc (5V)
    ADMUX = (1 << REFS0) | (canal & 0x07);
    _delay_us(20); // Tiempo para estabilizar el multiplexor

    // 2. Conversión de descarte (purga la carga residual del canal anterior)
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
        ;

    // 3. Conversión real (señal pura del canal seleccionado)
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
        ;

    return ADC;
}

// -----------------------------------------------------------------------------
// 3. ACTUALIZACIÓN DE LA BARRA DE 5 LEDS
// -----------------------------------------------------------------------------
static void bms_actualizar_barra_leds(uint16_t mv)
{
    // Determinamos cuántos LEDs encender (del 0 al 5) según los milivoltios
    uint8_t nivel = 0;
    if (mv >= 4000)
        nivel = 5; // Celda 100%
    else if (mv >= 3700)
        nivel = 4; // Celda 80%
    else if (mv >= 3400)
        nivel = 3; // Celda 60%
    else if (mv >= 3100)
        nivel = 2; // Celda 40%
    else if (mv >= 2800)
        nivel = 1; // Celda 20%
    else
        nivel = 0; // Celda descargada

    // Máscara dinámica: si nivel = 3 -> (1 << 3) - 1 = 0b00000111 (enciende PB0, PB1, PB2)
    uint8_t mascara_leds = (uint8_t)((1U << nivel) - 1U) & 0x1F;
    PORTB = (PORTB & ~0x1F) | mascara_leds;
}

// -----------------------------------------------------------------------------
// 4. PROGRAMA PRINCIPAL
// -----------------------------------------------------------------------------
int main(void)
{
    bms_hal_init();
    bms_init_queue(&cola_bms);

    bool sistema_bloqueado = false;

    while (1)
    {
        // --- GESTIÓN DE REARME MANUAL ---
        // Si el sistema se bloqueó por alarma, solo se reacciona si se pulsa PD2
        bool boton_rearme = !(PIND & (1 << 2));
        if (sistema_bloqueado && boton_rearme)
        {
            sistema_bloqueado = false;
            PORTD |= (1 << 7);  // Reactivar Relé
            PORTD &= ~(1 << 6); // Silenciar Buzzer
        }

        // --- ACTO 1: ADQUISICIÓN DE SENSORES ---
        uint16_t adc_pot = bms_adc_read(0);  // A0: Potenciómetro
        uint16_t adc_lm35 = bms_adc_read(1); // A1: Sensor de Temperatura

        uint16_t milivoltios = 2700 + (uint16_t)(((uint32_t)adc_pot * 1500UL) / 1023UL);
        uint8_t temperatura = (uint8_t)(((uint32_t)adc_lm35 * 500UL) / 1024UL);

        // Lectura digital de seguridad:
        // Fuego: Activo en bajo (0V = Llama detectada)
        bool fuego_detectado = !(PIND & (1 << 3));

        // Tapa/Gabinete: Activo en alto (5V = Gabinete abierto / Tamper disparado)
        bool tapa_abierta = (PIND & (1 << 4));

        // --- ACTO 2: CONSTRUCCIÓN Y ENCOLADO DE TRAMA ---
        uint8_t flags = 0;

        if (fuego_detectado)
            flags |= 0x08; // Bit 31 (Fuego)
        if (temperatura > 55)
            flags |= 0x04; // Bit 30 (Alerta térmica industrial)
        if (tapa_abierta)
            flags |= 0x02; // Bit 29 (Gabinete abierto)
        if (milivoltios > 4150)
            flags |= 0x01; // Bit 28 (Sobretensión)

        BmsTelemetry_t telemetria_tx = {
            .milivoltios = milivoltios,
            .id_celda = 1,
            .temperatura = temperatura,
            .distancia_tapa = tapa_abierta ? 1 : 0,
            .flags_criticos = flags};

        bms_enqueue_telemetry(&cola_bms, &telemetria_tx);

        // --- ACTO 3: CONSUMO Y AUDITORÍA ---
        BmsTelemetry_t telemetria_rx;
        BmsStatus_t status = bms_dequeue_and_process(&cola_bms, &telemetria_rx);

        // --- ACTO 4: ACCIONES FÍSICAS ---
        if (status == BMS_ALARMA_CRITICA || sistema_bloqueado)
        {
            sistema_bloqueado = true;
            PORTD &= ~(1 << 7); // ABRIR RELÉ (Corte total)
            PORTD |= (1 << 6);  // DISPARAR BUZZER
            PORTB &= ~0x1F;     // APAGAR LEDS
        }
        else
        {
            // Operación normal segura: actualizar barra visual con los datos recibidos
            bms_actualizar_barra_leds(telemetria_rx.milivoltios);
        }

        _delay_ms(100); // Muestreo a 10 Hz (100 ms)
    }

    return 0;
}