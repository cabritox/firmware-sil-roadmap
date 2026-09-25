# Semana 2: Operaciones a Nivel de Bit, Máscaras y Macros

Este directorio contiene la segunda fase del Roadmap de Firmware. El objetivo de esta semana es abandonar las operaciones aritméticas tradicionales y aprender a manipular la memoria a nivel de bit (1s y 0s), el verdadero lenguaje de los registros de hardware y los protocolos de comunicación.

## 🎯 Objetivo de la Semana
Dominar el empaquetado y desempaquetado de memoria para construir un Decodificador de Tramas y desplegarlo en silicio real. En lugar de gastar bytes enteros para variables booleanas, aprenderemos a comprimir múltiples estados y valores dentro de un solo registro de hardware utilizando operadores Bitwise y Macros industriales.

## 🧠 Mapa de Ruta por Día

* **Día 1 - El Arsenal Booleano (Operadores Básicos):** 
  Fundamentos de las compuertas lógicas en C. Uso de `&` (AND), `|` (OR), `^` (XOR) y `~` (NOT) para encender, apagar o alternar (toggle) pines específicos sin alterar el resto del registro.
* **Día 2 - Desplazamientos (Shifts) y Máscaras Dinámicas:** 
  Uso de los operadores de corrimiento a la izquierda (`<<`) y derecha (`>>`). Creación de máscaras de bits al vuelo para apuntar a pines específicos.
* **Día 3 - Abstracción de Hardware (Macros HAL):** 
  Transición del código crudo a la industria. Creación de directivas `#define` paramétricas (`BIT_SET`, `BIT_CLEAR`, `BIT_TOGGLE`, `BIT_CHECK`) para construir nuestra primera capa de abstracción de hardware.
* **Día 4 - Empaquetado de Datos (Construcción de Tramas):** 
  Fusión de múltiples variables pequeñas (IDs, flags de estado, cargas útiles) dentro de un único registro consolidado de 32 bits (`uint32_t`) utilizando máscaras compuestas (`BIT_MASK` y `PACK_FIELD`).
* **Día 5 - Desempaquetado, Read-Modify-Write y Auditoría de Tramas:** 
  El proceso inverso y validación en recepción. Extracción quirúrgica de campos desde un bloque de 32 bits en un bus "ciego" mediante máscaras dinámicas (`BIT_MASK`, `PACK_FIELD`, `UNPACK_FIELD`) y simulación de bucle cerrado (Loopback TX/RX). Incluye actualización de registros en caliente mediante el ciclo *Read-Modify-Write* (`CLEAR_FIELD`, `MODIFY_FIELD`) y barreras de validación (*Bit Testing* con `TEST_ANY_MASK` y `TEST_ALL_MASK`) para detectar colisiones de bus y flags críticos antes de procesar la carga útil (`assert`).
* **Día 6 - Arquitectura de Software y Banco de Tortura (`BMS Guard Core`):** 
  Desarrollo desde cero del código modular (`.h` y `.c`) del sistema **BMS Guard** en C puro. Acoplamiento del codificador/decodificador de 32 bits con el `RingBuffer32` (Semana 1), encapsulamiento de auditorías con funciones `static`, manejo de códigos de retorno estrictos (`enum`) y certificación lógica en consola mediante un banco de pruebas destructivo (`<assert.h>`).
* **Día 7 - Integración Bare-Metal y Despliegue Físico (`BMS Guard Hardware`):** 
  Transición del código validado al silicio real sobre una placa Arduino Uno R3 compatible[cite: 1]. Creación de la capa de mapeo de registros físicos AVR (`DDRB`, `PORTB`, `DDRD`, `PORTD`, `PIND`), cableado de sensores y actuadores en la Protoboard 830 puntos[cite: 1], y ejecución del lazo de protección e instrumentación en tiempo real.

---

## 🚀 Proyecto Integrador (Días 6 y 7)
**Decodificador Binario de Tramas y Estación de Protección de Celdas (BMS Guard)**

El entregable final se divide en dos fases de ingeniería: primero la construcción y validación matemática del firmware en consola (**Día 6**), y luego su conexión física con sensores y actuadores reales (**Día 7**).

**Mapa de Memoria de la Trama (`uint32_t`):**
* **Bits [3:0] (4 bits):** `id_celda` (Identificador del nodo sensor).
* **Bits [15:4] (12 bits):** `milivoltios` (Lectura de tensión de la celda).
* **Bits [23:16] (8 bits):** `temperatura` (Lectura térmica en °C).
* **Bits [27:24] (4 bits):** `distancia_tapa` (Estado del gabinete de seguridad).
* **Bits [31:28] (4 bits):** `flags_criticos` (`Bit 28`: Sobretensión, `Bit 29`: Gabinete abierto, `Bit 30`: Alerta térmica, `Bit 31`: Fuego/Ignición).

### Fase A: Arquitectura Modular y Simulación QA (Día 6)
* **Contrato y Motor Lógico (`bms_protocol.h` / `bms_protocol.c`):** Prevención de contaminación cruzada entre bits vecinos mediante generación de máscaras al vuelo (`BIT_MASK`, `PACK_FIELD`, `UNPACK_FIELD`, `MODIFY_FIELD`) sin números mágicos, encolando paquetes de forma segura en el `RingBuffer32`.
* **Auditoría Previa de Colisiones:** Inspección en tiempo constante $O(1)$ de la máscara de errores (`Bits [31:28]`) con `TEST_ANY_MASK` antes de desempaquetar la carga útil, devolviendo estados estrictos (`BMS_OK`, `BMS_ERR_NULL`, `BMS_ERR_VACIO`, `BMS_ALARMA_CRITICA`).
* **Banco de Tortura (`test_bms.c`):** Pruebas automatizadas en PC para auditar punteros `NULL`, saturación de cola circular, inyección de basura electrónica en bits altos y disparo de cortes de emergencia simulados.

### Fase B: Montaje Físico y Registros AVR (Día 7)
* **Entradas Físicas (Sensores):** Montaje en la Protoboard 830 puntos utilizando el Cable de prototipo M-M y el cable Hembra a macho dupond[cite: 1] para adquirir señales del sensor LM35[cite: 1], el Potenciómetro[cite: 1], el Sensor de Llama o fuego[cite: 1], el key button con filtro antirrebote por software[cite: 1] y el sensor de proximidad.
* **Salidas Físicas (Actuadores):** Control directo por registros sobre los 5 Diodos LED protegidos con el Pack Resistencia (barra de tensión mediante máscaras dinámicas)[cite: 1], alerta acústica con el Buzzer[cite: 1] y aislamiento físico del circuito mediante el Módulo de Relé[cite: 1].

---

### ⚙️ Instrucciones de Compilación y Ejecución

#### 1. Ejecutar el Banco de Tortura en PC (Día 6)
```bash
cd dia_6_bms_core
gcc -Wall -Wextra -Werror test_bms.c bms_protocol.c -o bms_test
./bms_test
```

#### 2. Compilar y Flashear en Hardware Real (Día 7)
Conectando la placa mediante el Cable USB tipo B para Arduino[cite: 1]:
```bash
cd dia_7_bms_hardware
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -Wall -Wextra main.c bms_protocol.c -o bms_firmware.elf
avr-objcopy -O ihex -R .eeprom bms_firmware.elf bms_firmware.hex
avrdude -c arduino -p m328p -P /dev/ttyACM0 -b 115200 -U flash:w:bms_firmware.hex:i
```