# 🖥️ Firmware Engineering & IoT - Software-in-the-Loop (SIL)

> Un registro de 12 meses documentando la transición hacia la Ingeniería de Sistemas Embebidos y Firmware, utilizando exclusivamente metodologías de simulación de grado industrial (Software-in-the-Loop).

Este repositorio contiene el código, las arquitecturas y las validaciones de un plan de estudios intensivo centrado en sistemas de recursos finitos, sistemas operativos en tiempo real (RTOS), redes IoT y silicio virtual. El objetivo es eliminar la dependencia de hardware físico inicial mediante emulación determinista.

## 🛠️ Stack Tecnológico y Entorno Virtual

* **Lenguajes:** C (C99/C11), C++ moderno
* **Sistemas Operativos (RTOS):** FreeRTOS, Zephyr RTOS
* **Frameworks:** ESP-IDF, MCUboot, Unity/CMock (TDD)
* **Simuladores (SIL):** Wokwi, QEMU (ARM), Renode (Antmicro)
* **Protocolos:** UART, I2C, SPI, TCP/IP, MQTT, BLE, TLS (mbedTLS)

## 📂 Estructura del Repositorio

```text
├── phase_1_abstract_c/      # Fundamentos de C, Memoria y Periféricos simulados
├── phase_2_esp32_iot/       # FreeRTOS, ESP-IDF, Conectividad y Telemetría
├── phase_3_zephyr_renode/   # Zephyr RTOS, Device Tree, BLE y Gemelos Digitales
├── phase_4_production/      # Bootloaders, OTA, CI/CD y Pruebas Unitarias
└── docs/                    # Diagramas de arquitectura, capturas de PulseView y datasheets
