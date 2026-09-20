# Semana 1: Fundamentos de C y Gestión de Memoria Físicamente Mapeada

Este directorio contiene la primera fase del Roadmap de Firmware (Phase 1: Abstract C). El objetivo de esta semana fue abandonar las abstracciones de los lenguajes de alto nivel y aprender a manipular, auditar y proteger la memoria RAM a nivel físico, sentando las bases para el desarrollo de sistemas embebidos seguros.

## 🎯 Objetivo de la Semana
Construir una arquitectura modular en C capaz de gestionar datos de forma continua y segura, previniendo colapsos de memoria (HardFaults) mediante programación defensiva y pruebas de estrés automatizadas.

## 🧠 Conceptos Dominados por Día

* **Día 1 - Topología de Memoria (Stack vs. Heap):** Mapeo de direcciones físicas mediante el operador `&` y el formato `%p` para entender la separación espacial entre la memoria estática (Data Segment) y las variables locales (Stack).
* **Día 2 - Aritmética de Punteros:** Manipulación de memoria en bruto mediante saltos físicos `*(ptr + offset)`, eliminando la dependencia del azúcar sintáctico de los índices de arreglos `[]`.
* **Día 3 - Lógica Circular:** Implementación del operador módulo (`%`) para el *wrap-around* matemático, permitiendo el reciclaje infinito de arreglos estáticos sin riesgo de desbordamiento.
* **Día 4 - Arquitectura Modular:** Separación estricta entre Contratos de Hardware (`.h` con *include guards*, `struct`, `enum`) y Fábricas Lógicas (`.c`).
* **Día 5 - Programación Defensiva:** Implementación de Cláusulas de Guardia (Early Returns) para blindar funciones contra inyecciones de punteros `NULL` e interceptar operaciones radiactivas antes de tocar el procesador.
* **Día 6 - Bancos de Pruebas (Test Harness):** Uso de la macro `<assert.h>` para ejecutar pruebas destructivas automatizadas en `main.c`, evaluando casos límite (Edge Cases) matemáticos y de desbordamiento.

## 🚀 Entregable Final (Día 7)
**Búfer Estático SPSC (Single Producer, Single Consumer)**

El proyecto integrador de la semana es la implementación de un *Ring Buffer* de grado industrial. Es la estructura fundamental para recibir telemetría de sensores sin asignar memoria dinámicamente y sin perder datos.

**Características del Módulo:**
* Tamaño estático y seguro en tiempo de compilación.
* API con códigos de estado estrictos (`BUFFER_OK`, `BUFFER_LLENO`, `BUFFER_VACIO`).
* Blindaje total contra ataques de *Overflow* (desbordamiento), *Underflow* (inanición) y punteros inválidos.

### ⚙️ Instrucciones de Compilación y Ejecución

Para ejecutar el banco de pruebas automatizado del Búfer SPSC:

1. Navega a la carpeta del entregable:
   ```bash
   cd dia_7_entregable