# Base de Conocimiento: Aritmética de Punteros y Acceso a Ras de Metal

Este documento registra la mecánica física de los punteros en C, eliminando la abstracción de los corchetes (`[]`) y exponiendo cómo el procesador interactúa directamente con la memoria SRAM y Flash.

## 1. Naturaleza Física del Puntero
*   **Definición Estricta:** Un puntero no almacena datos de usuario, almacena una coordenada física hexadecimal (ej. `0x7ffe...`).
*   **Tamaño Universal:** Sin importar si apunta a una variable de 1 byte o a un búfer de 10 Megabytes, el puntero en sí siempre pesa lo mismo (usualmente 4 bytes en arquitecturas de 32 bits, como un ESP32 o ARM Cortex-M).

## 2. La Ley del Salto (Aritmética)
La instrucción de sumar posiciones a un puntero (`ptr + i`) no avanza bytes de uno en uno, sino que se multiplica por el tamaño del tipo de dato:
*   Si el puntero es `uint8_t*`, `+1` mueve el láser del procesador **1 byte** físico.
*   Si el puntero es `uint32_t*`, `+1` mueve el láser del procesador **4 bytes** físicos de golpe.
*   Usar el tipo de puntero equivocado provocará que leas variables que no te corresponden, corrompiendo la extracción de datos.

## 3. La Ley de la Mordida (Desreferencia `*`)
El asterisco `*()` es la orden de apertura de memoria. El tipo de puntero dicta cuánta memoria se leerá o sobrescribirá en ese instante:
*   `*(uint8_t*) = 0x00;` inyecta ceros en exactamente 1 celda de memoria.
*   `*(uint32_t*) = 0x00;` inyecta ceros en 4 celdas consecutivas.
*   **Peligro en Firmware:** Sobrescribir con la "mordida" equivocada destruirá los bytes adyacentes de otros sensores o variables críticas.

## 4. Reglas de Transferencia de Datos
El diseño de firmware exige elegir mecánicamente cómo viajan los datos entre funciones:
*   **Copia Directa (Paso por Valor):** Se usa estrictamente para variables pequeñas (1 a 4 bytes) que solo necesitan ser *leídas*. Es barato para el procesador y protege la variable original.
*   **Paso por Referencia (Punteros):** Se usa obligatoriamente para:
    1.  **Arreglos y Búferes:** Evita clonar memoria masiva, ahorrando ciclos de reloj y RAM.
    2.  **Sobrescritura Remota:** Permite que una función cruce su *scope* y modifique variables que viven en el `main()`.
    3.  **Hardware Registros:** Es la única forma de apuntar el procesador a la dirección hexadecimal física de un pin o un sensor.