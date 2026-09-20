## Día 7: Entregable Final - Búfer Estático SPSC

El cierre de la Semana 1 culmina con la construcción de una estructura de memoria de grado industrial: un Ring Buffer SPSC (Single Producer, Single Consumer). Este módulo es el corazón de la adquisición de datos en hardware crítico (como la telemetría de celdas en un BMS o la lectura de sensores ADC), permitiendo encolar y procesar información en tiempo real sin riesgo de desbordamiento ni necesidad de asignación dinámica de memoria.

**Arquitectura Implementada:**
*   **Contrato Topológico (`buffer.h`):** Definición del `struct` de datos estáticos y un `enum` con códigos de diagnóstico estrictos (`BUFFER_OK`, `BUFFER_LLENO`, etc.).
*   **Motor de Silicio (`buffer.c`):** Manipulación de memoria exclusivamente mediante aritmética de punteros (prohibición de índices `[]`) y control de avance circular iterativo utilizando el operador módulo (`%`).
*   **Defensa Activa:** Blindaje total de las funciones `push` y `pop` mediante cláusulas de guardia para interceptar ataques de punteros nulos y evitar colapsos físicos del procesador (HardFault).
*   **Test Harness (`main.c`):** Banco de pruebas automatizado mediante `<assert.h>`, diseñado para bombardear el búfer forzando estados de desbordamiento (Overflow) e inanición (Underflow).