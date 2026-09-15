# Bitácora de Ingeniería: Semana 1 - Memoria, Punteros y Estructuras Estáticas

## 🎯 Plan de Ejecución Diario
- [ ] **Día 1 (Stack vs Heap):** Imprimir y comparar direcciones físicas de memoria global vs local.
- [ ] **Día 2 (Aritmética de punteros):** Iterar arreglos utilizando `*(ptr + i)` sin sintaxis de índices `[]`.
- [ ] **Día 3 (Lógica Circular):** Mapeo matemático del operador módulo (`%`) para el *wrap-around*.
- [ ] **Día 4 (Interfaz .h):** Definir el contrato de datos (`struct`) y los códigos de estado (`enum`).
- [ ] **Día 5 (Implementación .c):** Lógica estricta de `push` y `pop` con validación de punteros nulos.
- [ ] **Día 6 (Casos Límite):** Ejecutar `main.c` forzando y previniendo desbordamientos de memoria.
- [ ] **Día 7 (Auditoría):** Revisión de vulnerabilidades y primer commit de código funcional.

---

## 📝 Registro de Decisiones y Errores (Post-mortems)

### Día 1: Arquitectura de Memoria (Stack vs Heap)
* **Concepto asimilado:** *Se verificó mediante las direcciones de memoria que el Stack reside en posiciones altas (0x7f...) y crece hacia abajo (de fff a fce), mientras que las variables globales viven en posiciones bajas (0x59...). Descartamos malloc porque en un mapa de memoria limitado, el crecimiento dinámico provoca colisiones inminentes.*
* **Ejecución y Bloqueos:** *Al compilar, introduje un espacio accidental en el comando (gcc dia1.c -o dia 1). Esto me demostró que la interfaz de línea de comandos de C es estrictamente literal: el enlazador de GCC (ld) interpretó la salida como "dia" y asumió que el "1" era un segundo archivo fuente que debía incluir, arrojando el error fatal /usr/bin/ld: no se puede encontrar 1. Se solucionó corrigiendo la sintaxis del ejecutable a una sola palabra ininterrumpida (-o dia1).*
* **Análisis de Binarios(size):** *Modifiqué el tamaño del búfer estático a 1 millón de bytes. Comprobé que la sección .bss absorbe la carga de RAM sin aumentar el peso del código en la memoria Flash (text). Entendí que un fallo común en firmware es solicitar un tamaño de .bss que excede la RAM física; el código compilará, pero el hardware colapsará en el Startup Code antes de ejecutar main().*
* **Simulacro Físico de Memoria:** *Comprendí que las variables globales inmutables (const) se quedan en Flash (.rodata). Las globales inicializadas van a .data (consumen Flash y RAM), las globales vacías a .bss (solo consumen RAM al arrancar), y las locales viven temporalmente en el Stack mediante el movimiento electromagnético del Stack Pointer.*

### Día 2: Aritmética de Punteros
* **Aritmética vs Sintaxis:** *Comprobé que arreglo[i] es solo una ilusión visual. Físicamente, el compilador lo traduce como *(puntero + i).*
* **El salto fisico:** *Al sumar +1 a un puntero, la CPU no avanza 1 byte literal. Avanza una cantidad de bytes igual al tamaño del tipo de dato (sizeof(tipo)). Un puntero uint32_t* saltará de a 4 bytes, mientras que un uint8_t* saltará de a 1 byte.*
* **Simulacro de transferencia:** *Comprendí la regla de oro para el paso de datos. Uso copia directa para variables pequeñas (1 a 4 bytes) que solo necesitan ser leídas. Uso punteros para arreglos grandes (evitando clonar memoria masiva) o cuando necesito que una función actualice variables que viven en el main() (superando el límite de un solo return).*
***Sobrescritura remota:** *Logré limpiar ruido de una señal mutando la memoria in-situ con *(ptr + i). Entendí que el tipo de puntero (uint8_t*) es lo que dicta el tamaño de la "mordida" física (1 byte) al momento de sobrescribir, evitando destruir la memoria adyacente.*

### Día 3: Lógica Circular y Wrap-around
* **Concepto asimilado:** *(Demuestra matemáticamente por qué la operación `(head + 1) % capacity` garantiza que el índice nunca exceda los límites de la memoria reservada).*
* **Ejecución y Bloqueos:** *(Dibuja o explica un caso límite de la lógica: ¿qué ocurre exactamente con los índices cuando el búfer se llena por completo?).*

### Día 4: Contrato de Datos (Interfaz `.h`)
* **Concepto asimilado:** *(Justifica por qué es vital separar la definición de la estructura de la asignación real de memoria. ¿Por qué el archivo de cabecera no reserva RAM?).*
* **Ejecución y Bloqueos:** *(Registra problemas con las guardas de inclusión `#ifndef` o con la importación de `<stdint.h>` para asegurar tipos de tamaño exacto).*

### Día 5: Implementación Segura (`.c`)
* **Concepto asimilado:** *(Explica tu estrategia de validación defensiva al inicio de cada función. ¿Por qué comprobar punteros nulos es el primer escudo contra un colapso del sistema?).*
* **Ejecución y Bloqueos:** *(Documenta cualquier error lógico al manipular el estado interno de `count`, `head` o `tail` al insertar datos).*

### Día 6: Pruebas de Estrés y Casos Límite (`main.c`)
* **Concepto asimilado:** *(Resume cómo responde tu arquitectura de software cuando se intenta hacer `push` destructivo a un búfer lleno).*
* **Ejecución y Bloqueos:** *(Copia y pega aquí la salida de la consola de tu ejecución demostrando que el desbordamiento fue interceptado y prevenido correctamente).*

### Día 7: Auditoría y Cierre
* **Revisión estática:** *(Anota las correcciones de seguridad o vulnerabilidades detectadas durante la revisión del código).*
* **Estado Final:** Código funcional integrado al control de versiones. Base sólida de manipulación de memoria establecida para comenzar con operaciones a nivel de bit (Semana 2).