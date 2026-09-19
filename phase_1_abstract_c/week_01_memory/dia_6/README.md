# Resumen Día 6: Pruebas de Estrés y Casos Límite

En el desarrollo industrial de firmware, el código no se valida "mirando" resultados en la terminal, sino sometiéndolo a un banco de pruebas automatizado (Test Harness). Este día se centró en la creación de un entorno de aserciones para auditar módulos antes de su implementación física.

**Conceptos Clave:**
*   **Aserciones (`<assert.h>`):** Implementación de macros de evaluación estricta. Permiten que la máquina valide automáticamente la lógica matemática. Si una aserción es falsa, el programa aborta inmediatamente indicando la línea del fallo.
*   **Casos Límite (Edge Cases):** Metodología de bombardeo de funciones utilizando los umbrales de quiebre de la lógica.
    *   *Fronteras Lógicas:* Validación de los umbrales inmediatos (ej. 449 y 450 para un límite de 45).
    *   *Estrés de Datos:* Inyección del cero absoluto (0) y el máximo valor de desbordamiento de la arquitectura (`65535` para 16 bits).
    *   *Vectores Destructivos:* Inyección de punteros `NULL` para auditar la solidez de las cláusulas de guardia (Early Returns).
*   **Aislamiento de Pruebas:** Comprensión de que el código de prueba (`assert`) vive exclusivamente en el entorno de desarrollo y nunca se incluye en el binario de producción para evitar colapsos del microcontrolador.