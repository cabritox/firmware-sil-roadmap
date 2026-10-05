# Semana 4: Concurrencia Hardware, ISRs y Máquinas de Estado

## 🎯 Objetivo de la Semana
Dominar la interacción entre el flujo principal del programa (`main`) y las interrupciones asíncronas de hardware (ISRs). Aprender a estructurar código no bloqueante y escalable utilizando punteros a funciones para implementar Máquinas de Estados Finitos (FSM).

## 🛠 Enfoque Técnico
* El calificador `volatile` y la optimización del compilador.
* Rutinas de Servicio de Interrupción (ISRs) y atomicidad.
* Punteros a funciones y *callbacks*.
* Diseño de FSM (Finite State Machines) guiadas por tablas.

## 🚀 Entregable Final
Máquina de estados abstracta interactiva por teclado (Simulación de un menú/sistema de control mediante la terminal).

## 📅 Plan de Estudio Diario
* **Día 1:** El calificador `volatile` y la trampa del optimizador.
* **Día 2:** Anatomía de una ISR y variables compartidas.
* **Día 3:** Punteros a funciones: La memoria ejecutable.
* **Día 4:** Tablas de despacho (*Dispatch Tables*) para evitar el código espagueti.
* **Día 5:** Proyecto Entregable: Máquina de Estados (FSM) basada en eventos.