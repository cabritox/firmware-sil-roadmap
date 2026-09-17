# Base de Conocimiento: Arquitectura Modular, Contratos (.h) y el Linker

Este documento establece las reglas de arquitectura para dividir un proyecto monolítico en módulos independientes, garantizando una compilación segura y el control absoluto de la memoria en sistemas embebidos.

## 1. El Contrato de Datos (.h) y la Fábrica (.c)
En hardware, la memoria se gestiona al milímetro. Por ello, la definición de un objeto y su asignación en la RAM deben estar estrictamente separadas.
*   **Archivo `.h` (El Plano):** Funciona como un contrato topológico. Define las firmas de las funciones (prototipos) y los moldes de memoria (`structs`). No contiene lógica ejecutable ni reserva un solo byte de RAM.
*   **Include Guards (El Escudo):** Todo contrato debe estar protegido por macros (`#ifndef`, `#define`, `#endif`). Esto evita que el preprocesador pegue el texto múltiples veces, lo cual agotaría la memoria y haría colapsar al compilador por redefinición.
*   **Archivo `.c` (La Fábrica):** Importa su respectivo contrato (`#include "archivo.h"`) y contiene la lógica real que se traducirá a código máquina, instanciando la memoria física.

## 2. El Enlazador (Linker) y la Compilación
El comando `gcc` ejecuta fases críticas en milisegundos para conectar los módulos físicos:
*   **El Preprocesador:** Lee los comandos `#include` y pega literalmente el texto del contrato dentro del archivo que lo solicita.
*   **El Agujero de Memoria:** Cuando el código llama a una función externa, el compilador confía en el `.h` y deja un "agujero" direccional, sabiendo que la función existe pero desconociendo su ubicación física.
*   **El Linker (El Pegamento):** Al pasar todos los archivos fuente al compilador (ej. `gcc main.c sensor.c -o modulo`), el Linker toma los binarios individuales y los fusiona. Conecta los "agujeros" direccionales con las direcciones de memoria reales de las funciones.
*   **Error Fatal (`undefined reference`):** Ocurre cuando el código promete una función en un `.h`, pero nunca se le entrega al Linker el `.c` que contiene la lógica para pegarlo.

## 3. Acceso a Memoria: Punteros y el Operador Flecha (`->`)
Para no desperdiciar RAM pasando copias de paquetes de telemetría entre funciones, se transfiere únicamente la dirección inicial.
*   Al recibir un `struct` por referencia a través de un puntero (`*`), se abandona el acceso por punto (`.`).
*   Se utiliza el operador flecha (`->`) para que el procesador "salte" por el puntero y acceda a los bytes físicos específicos dentro del bloque de memoria original.

## 4. Seguridad Lógica: Estados de Hardware Exclusivos
Los procesadores ejecutan instrucciones secuencialmente y sin cuestionar el contexto físico.
*   **Condicionales Aislados:** Las variables de estado crítico (ej. alarmas térmicas, encendido de relés) controladas por umbrales deben aislarse forzosamente con bloques `if / else`.
*   **Falla Secuencial:** Escribir un reinicio de estado fuera de un bloque condicional provoca que la CPU obedezca ciegamente, aplastando una alerta crítica en una fracción de microsegundo justo después de haberla activado, generando fallos catastróficos silenciosos en el dispositivo físico.