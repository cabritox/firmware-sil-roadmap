# Semana 6: El Tiempo y las Interrupciones (ISRs y Temporizadores)

## 🎯 Objetivo de la Semana
Dejar atrás la técnica de "polling" (revisar continuamente el estado de un pin en un bucle `while`) y aprender a delegar la gestión del tiempo y los eventos asíncronos directamente al hardware del microcontrolador. Dominaremos las Rutinas de Servicio de Interrupción (ISR) y la configuración de temporizadores (Timers) mediante la manipulación de sus registros de control en C bare-metal.

## 🛠 Enfoque Técnico
-   Arquitectura de interrupciones de hardware (Hardware Interrupts).
-   Vectores de interrupción y rutinas ISR (Interrupt Service Routines).
-   Configuración de temporizadores de hardware (Timers / Counters).
-   Modo CTC (Clear Timer on Compare Match) para precisión de tiempo.
-   El rol crítico del calificador `volatile` en variables compartidas con ISRs.

## 🚀 Entregable Final
Un firmware simulado en Wokwi que utiliza un temporizador de hardware de alta precisión para disparar una interrupción (ISR) a intervalos exactos, controlando un LED, sin bloquear el flujo principal de ejecución en la función `main`.

## 📅 Plan de Estudio Diario

*   **Día 1: El concepto de Interrupción y los Vectores (IVT):** Qué es una interrupción, cómo el hardware detiene al procesador y la anatomía de una Rutina de Servicio de Interrupción (ISR) en C.
*   **Día 2: Anatomía de un Temporizador (Timer 1):** Exploración en el Datasheet de los registros clave de un temporizador de 16 bits (TCNT1, TCCR1A, TCCR1B) y el concepto de *Prescaler* (divisor de reloj).
*   **Día 3: Temporización de Precisión (Modo CTC):** Cómo calcular el tiempo exacto. Uso del registro de comparación (OCR1A) para reiniciar el temporizador automáticamente al alcanzar un valor matemático.
*   **Día 4: Habilitación de la Interrupción del Temporizador:** Manipulación de los registros de máscara (TIMSK1) y la instrucción global de interrupciones (`sei()`), uniendo el Timer con la función ISR.
*   **Día 5: Proyecto Entregable (Temporizador ISR en Wokwi):** Construcción del código final con arquitectura de macros, configuración del Timer 1 en modo CTC a 1 Hz, y encendido/apagado de un LED dentro de la ISR, testeado en el simulador Wokwi.