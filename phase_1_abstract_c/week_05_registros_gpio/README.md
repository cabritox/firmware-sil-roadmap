# Semana 5: El Hardware al Desnudo (Registros, Datasheets y GPIO)

## 🎯 Objetivo de la Semana
Aprender a leer la documentación técnica de los fabricantes (Datasheets) para descubrir las direcciones físicas de los periféricos y controlarlos utilizando C puro (Bare-Metal), prescindiendo por completo de librerías de alto nivel (como el framework de Arduino).

## 🛠 Enfoque Técnico
* Mapa de registros y datasheets.
* Memory-Mapped I/O (E/S Mapeada en Memoria).
* Manipulación de hardware mediante punteros absolutos.
* Aplicación de máscaras de bits para control de pines (GPIO).

## 🚀 Entregable Final
Configuración de GPIO en Wokwi por dirección de memoria (Lectura de botones físicos y accionamiento de LEDs manipulando los registros DDRx, PORTx y PINx).

## 📅 Plan de Estudio Diario
* **Día 1: Anatomía de un Datasheet y el Mapa de Memoria:** Cómo encontrar el tesoro (las direcciones hexadecimales de los periféricos).
* **Día 2: Memory-Mapped I/O:** Creando punteros en C que apuntan a transistores físicos.
* **Día 3: El Bisturí Bitwise:** Modificando registros de hardware usando máscaras (Set, Clear, Toggle, Check).
* **Día 4: Arquitectura de macros para hardware:** Definiendo tus propios nombres de registros de forma profesional (ej. `#define PORTB`).
* **Día 5: Proyecto Entregable (Bare-Metal en Wokwi):** Configurar y controlar pines GPIO mediante direcciones de memoria puras sin usar librerías externas.
## 🎮 Simulación Interactiva
[👉 Clic aquí para ver y ejecutar el circuito en Wokwi](https://wokwi.com/projects/478068625829937153)