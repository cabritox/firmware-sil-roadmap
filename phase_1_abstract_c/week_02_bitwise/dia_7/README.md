# BMS Firmware Bare-Metal — ATmega328P (Día 7)

Implementación y validación en hardware de un **Sistema de Gestión de Baterías (BMS)** embebido para microcontroladores AVR (ATmega328P / Arduino Uno) desarrollado puramente en **C bare-metal** (sin dependencias del framework de Arduino). 

El sistema gestiona la adquisición de variables analógicas, entradas digitales de seguridad industrial, enclavamiento por fallo (*Safety Latch*), corte de potencia galvánico por relé, interfaz visual de carga (SoC) y una arquitectura modular desacoplada mediante un búfer circular de telemetría de 32 bits (*Ring Buffer*).

---

## 1. Mapeo Completo de Hardware y Pines

Para evitar interferencias con el transceptor serie USB del programador (`/dev/ttyUSB0`), los pines `PD0` y `PD1` se mantienen estrictamente aislados de la circuitería externa.

| Pin Físico | Registro AVR | Dirección | Periférico / Señal | Tipo de Señal / Lógica | Función en el BMS |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **`A0`** | `PC0` | Entrada | Potenciómetro | Analógica ($0\text{ a }5\text{ V}$) | Simulación de tensión Li-ion ($2700\text{ a }4200\text{ mV}$) |
| **`A1`** | `PC1` | Entrada | Sensor LM35 | Analógica ($10\text{ mV}/^\circ\text{C}$) | Monitoreo térmico directo de celda |
| **`Pin 2`** | `PD2` | Entrada | Pulsador de Rearme | Digital / Pull-Up Interno | Rearme manual obligatorio (*Safety Reset*) |
| **`Pin 3`** | `PD3` | Entrada | Sensor de Llama IR | Digital / Active-LOW | Detección óptica de incendio |
| **`Pin 4`** | `PD4` | Entrada | Sensor de Tapa | Digital / Active-HIGH | Detección de intrusión en gabinete |
| **`Pin 6`** | `PD6` | Salida | Buzzer Activo | Digital ($0\text{V} / 5\text{V}$) | Alarma sonora continua de fallo |
| **`Pin 7`** | `PD7` | Salida | Módulo Relé | Digital ($0\text{V} / 5\text{V}$) | Desconexión galvánica de seguridad |
| **`Pines 8-12`** | `PB0..PB4` | Salidas | Barra de 5 LEDs | Digital ($0\text{V} / 5\text{V}$) | Indicador visual de SoC ($20\%$ a $100\%$) |
| **`Pines 0-1`** | `PD0/PD1` | E/S | UART Serie | TTL Serial | Programación Flash y Telemetría futura |

---

## 2. Diagrama Eléctrico de Conexión

```text
                     ATmega328P (Arduino Uno)
                    ┌─────────────────────────┐
       +5V ─────────┤ 5V                      │
       GND ─────────┤ GND                     │
                    │                         │
  [Potenciómetro] ──┤ A0 (PC0)                │
     [LM35 Vout] ───┤ A1 (PC1)                │
                    │                         │
 [Pulsador Rearme]──┤ Pin 2 (PD2)             │   (Pata opuesta a GND)
   [Sensor Llama] ──┤ Pin 3 (PD3)             │   (DO del módulo LM393)
   [Sensor Tapa]  ──┤ Pin 4 (PD4)             │   (GND = Seguro, +5V = Abierto)
                    │                         │
     [Buzzer (+)] ──┤ Pin 6 (PD6)             │
    [Relé Signal] ──┤ Pin 7 (PD7)             │
                    │                         │
    LED 1 (20%)   ──┤ Pin 8  (PB0) ───[330Ω]──►|── GND
    LED 2 (40%)   ──┤ Pin 9  (PB1) ───[330Ω]──►|── GND
    LED 3 (60%)   ──┤ Pin 10 (PB2) ───[330Ω]──►|── GND
    LED 4 (80%)   ──┤ Pin 11 (PB3) ───[330Ω]──►|── GND
    LED 5 (100%)  ──┤ Pin 12 (PB4) ───[330Ω]──►|── GND
                    └─────────────────────────┘