# Spec-Driven Development: Timer1 CTC Interrupts (Bare-Metal AVR)

## 1. Contexto del Sistema
- **Plataforma:** ATmega328P (F_CPU = 16 MHz).
- **Objetivo:** Controlar el parpadeo de un LED a una frecuencia exacta de 1 Hz (1 segundo encendido, 1 segundo apagado) utilizando temporizadores de hardware e interrupciones, eliminando la técnica de polling.
- **Arquitectura:** C puro (Bare-Metal). Prohibido el uso del framework de Arduino o librerías bloqueantes.

## 2. Mapa de Memoria y Hardware (Capa 1)
El archivo `macros.h` debe implementar los punteros volátiles hacia las siguientes direcciones físicas exactas del Datasheet:
- **GPIO (Puerto B):** `PORTB` (0x25), `DDRB` (0x24), `PIN_LED` (Pin 5).
- **Timer 1 (Registros de Control - 8 bits):** `TCCR1A` (0x80), `TCCR1B` (0x81), `TIMSK1` (0x6F).
- **Timer 1 (Registros de Datos - 16 bits):** `TCNT1` (0x84), `OCR1A` (0x88).

## 3. Reglas de Negocio y Configuración (Capa 2)
El archivo `main.c` debe inicializar el hardware en el siguiente orden estricto antes del bucle infinito:
1.  **Reinicio:** Limpiar `TCCR1A`, `TCCR1B` y `TCNT1` a 0 por seguridad.
2.  **Frecuencia (Modo CTC):** Escribir el valor objetivo de `15624` en el registro `OCR1A` (calculado para 1 Hz con F_CPU 16MHz y Prescaler 1024).
3.  **Habilitación de Interrupción Local:** Inyectar un `1` lógico en el bit `OCIE1A` (Bit 1) del registro de máscara `TIMSK1`.
4.  **Arranque del Hardware:** Encender el bit `WGM12` (Bit 3) para modo CTC, y los bits `CS12` (Bit 2) y `CS10` (Bit 0) en `TCCR1B` para aplicar el Prescaler de 1024.
5.  **Interrupción Global:** Ejecutar `sei()` de `<avr/interrupt.h>` para quitar el bozal a la CPU.

## 4. Restricciones de Implementación (Constraints)
- **Macros Bitwise:** Se deben crear y utilizar exclusivamente macros puras para manipulación (`BIT_SET`, `BIT_CLEAR`, `BIT_TOGGLE`).
- **ISR:** El cambio de estado del LED (Toggle) debe ocurrir *exclusivamente* dentro de la macro `ISR(TIMER1_COMPA_vect)`.
- **Non-Blocking:** El bucle `while(true)` dentro de `int main(void)` debe estar 100% vacío. La CPU no debe ejecutar ninguna instrucción de espera.