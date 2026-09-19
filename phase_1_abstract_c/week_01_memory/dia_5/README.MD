# Base de Conocimiento: Programación Defensiva y Punteros Nulos

Este documento registra las técnicas de protección de memoria para evitar colapsos del sistema (HardFaults) ante entradas inválidas.

## 1. El Peligro del Puntero Nulo (NULL)
* En hardware, intentar escribir o leer en la dirección `0x00000000` (NULL) provoca un colapso físico inmediato del procesador (en Linux, un Segmentation Fault).
* El código nunca debe confiar ciegamente en los punteros que recibe como parámetros.

## 2. Cláusulas de Guardia (Early Return)
* Es una estructura lógica que se coloca al inicio de una función para auditar los parámetros de entrada.
* Si la validación falla (ej. `ptr == NULL`), se utiliza un `return;` temprano para expulsar al procesador de la función inmediatamente, abortando la operación antes de que intente acceder a la memoria.

## 3. Aislamiento de Librerías
* Comprobé que importar una herramienta en `main.c` (como `<stdio.h>`) no la hace disponible para los demás archivos del proyecto. Cada archivo `.c` es ciego y debe declarar sus propios `#include` de forma explícita.