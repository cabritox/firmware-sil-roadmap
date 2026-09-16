# Base de Conocimiento: Lógica Circular, Ring Buffers y Alineación de Memoria

Este documento registra los mecanismos arquitectónicos para gestionar flujos infinitos de datos en memorias finitas (SRAM) y las reglas de empaquetado físico de datos heterogéneos en C.

## 1. Lógica Circular y Ring Buffers
En sistemas embebidos, el programa nunca se detiene y la memoria no crece. Para evitar un desbordamiento catastrófico de memoria, se utiliza el patrón de **Búfer Circular**.
*   **Mecánica:** Un arreglo estático de tamaño fijo donde los datos nuevos sobrescriben a los más antiguos (wrap-around), garantizando un consumo de RAM constante ($O(1)$).
*   **Índices de Control:** Se utilizan dos punteros lógicos para evitar colisiones:
    *   **Head (Cabeza):** Controla dónde escribe el productor (ej. sensor).
    *   **Tail (Cola):** Controla dónde lee el consumidor (ej. antena Wi-Fi).
*   **Buffer Overrun:** Ocurre cuando el `Head` alcanza por detrás al `Tail` (`Head == Tail`), corrompiendo datos vivos que aún no habían sido procesados.

## 2. Optimización de Silicio (Módulo vs. Máscaras de Bits)
El cálculo del índice en un búfer circular dicta el rendimiento del procesador:
*   **El Método Lento (`%`):** Usar el operador módulo (ej. `indice % tamaño`) fuerza a la CPU a realizar una división por hardware, consumiendo decenas de ciclos de reloj.
*   **El Estándar Industrial (`&`):** Si el tamaño del búfer es una potencia de 2 (ej. 256, 512, 1024), el módulo se reemplaza por una máscara de bits `AND` (ej. `indice & 255`). Esto trunca los bits excedentes en exactamente **1 ciclo de reloj**.

## 3. Structs, Padding y Alineación de Memoria
En C, un `struct` no es una abstracción orientada a objetos; es un bloque físico y contiguo de SRAM fundido a medida.
*   **Memory Alignment:** Los procesadores de 32 bits leen la memoria en bloques de 4 bytes. Para evitar que una variable quede dividida entre dos bloques de lectura, el compilador interviene.
*   **Padding (Relleno):** El compilador inyecta bytes de memoria "muerta" o basura entre las variables para forzar que calcen en múltiplos de 4. Esto optimiza la velocidad de la CPU pero desperdicia memoria RAM valiosa.
*   **Regla de Arquitectura:** Para minimizar la inyección de *padding* y ahorrar memoria en microcontroladores, las variables dentro de un `struct` **siempre deben ordenarse de mayor a menor tamaño** (ej. `uint32_t` -> `uint16_t` -> `uint8_t`).