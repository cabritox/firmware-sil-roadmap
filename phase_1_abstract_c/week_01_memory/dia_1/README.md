# Base de Conocimiento: Arquitectura de Memoria a Ras de Metal

Este documento registra los fundamentos físicos de la memoria en sistemas embebidos (SRAM y Flash), eliminando las abstracciones de los lenguajes de alto nivel.

## 1. Topología del Silicio (SRAM vs. Flash)
*   **`.text` (Flash):** Instrucciones de código inmutables. El procesador solo lee de aquí.
*   **`.rodata` (Flash):** Variables inmutables (`const`). No consumen RAM.
*   **`.data` (Flash -> SRAM):** Variables globales inicializadas. Tienen un costo doble: ocupan Flash estando apagado el equipo, y el *Startup Code* (`crt0`) las copia a la SRAM al encender.
*   **`.bss` (SRAM):** Variables globales vacías o inicializadas en cero. No ocupan espacio en la Flash (el archivo compilado no pesa más). El *Startup Code* reserva la SRAM y la llena de ceros físicamente.

## 2. Mecánica del Stack (La Pila)
*   **Crecimiento Inverso:** El Stack se ubica en las direcciones más altas de la RAM (`0x7f...`) y crece hacia abajo, acercándose peligrosamente a la memoria estática si se sobredimensiona (*Stack Overflow*).
*   **El Stack Pointer (SP):** Las variables locales no se "destruyen" al terminar una función. El hardware simplemente mueve el puntero SP hacia arriba.
*   **Punteros Colgantes (*Dangling Pointers*):** Retornar la dirección de una variable local deja un mapa hacia "datos fantasma". El voltaje sigue ahí, pero será sobrescrito instantáneamente por la siguiente función o interrupción de hardware.

## 3. La Prohibición del Heap
*   En firmware crítico, la memoria dinámica (`malloc`/`free`) está prohibida.
*   **Fragmentación:** Ciclos constantes de asignación y liberación dejan bloques de memoria libres pero aislados. Un dispositivo encendido durante meses eventualmente colapsará al no encontrar bloques contiguos.
*   **Solución Embebida:** Calcular el peor escenario posible y reservar búferes de tamaño estricto y fijo en la sección `.bss`.

## 4. Auditoría de Compilación
*   El comando `size <binario>` revela el consumo exacto de las secciones `.text`, `.data` y `.bss` antes de transferir el código al microcontrolador. Un binario pequeño en disco duro aún puede colapsar el hardware si su `.bss` exige más SRAM de la que el chip posee físicamente.