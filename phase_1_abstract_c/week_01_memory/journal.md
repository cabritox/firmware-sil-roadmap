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

### Día 3: Lógica Circular y Ring Buffers
* **Supervivencia de Memoria:** *Comprobé que un sistema embebido requiere procesar datos infinitos en una memoria finita. La solución arquitectónica es el Búfer Circular (Ring Buffer), que sobrescribe los datos más antiguos garantizando un consumo de RAM constante y $O(1)$.*
* **El Truco del Wrap-around:** *Entendí que el límite físico se controla matemáticamente con el operador módulo (%). Sin embargo, en hardware crítico, el tamaño del arreglo debe ser una potencia de 2 para reemplazar el % (lento) por una máscara de bits & (1 ciclo de reloj).*
* **Structs y Padding:** *Descubrí que un struct en C no es abstracto, es un bloque de SRAM físico. El compilador inyecta "Padding" (memoria muerta) para que las variables calcen en múltiplos de 4 bytes, optimizando los ciclos de lectura de la CPU a costa de desperdiciar RAM.*
* **Regla de Optimización:** *Para minimizar la pérdida de memoria en sistemas embebidos, siempre debo ordenar los elementos del struct desde el tipo de dato más grande al más pequeño.*

### Día 4: Arquitectura Modular, Contratos (.h) y el Linker
* **Concepto asimilado:** *Es vital separar la definición (el .h) de la lógica ejecutable (el .c) porque en el diseño de hardware la gestión de SRAM es milimétrica. El archivo de cabecera funciona exclusivamente como un plano topológico y un contrato de confianza; no reserva un solo byte de RAM. Si un .h instanciara memoria física real, cada vez que otro módulo del sistema hiciera #include, el preprocesador duplicaría la variable, agotando la memoria disponible y provocando que el Linker colapse por "redefinición". El .h solo promete que la estructura existirá, permitiendo que el .c asigne la memoria en el silicio una única vez.*
* **Ejecución y Bloqueos:** *El bloqueo principal fue el error fatal del Linker (referencia sin definir). Comprobé que el #include no conecta archivos mágicamente, solo deja un "agujero" en el compilador, siendo obligatorio inyectar todos los archivos .c en el comando gcc para que el Linker ensamble los binarios. También registré que usar <stdint.h> en los contratos es innegociable: garantiza que los datos de telemetría midan los bytes exactos (ej. uint16_t para ADC) independientemente de la arquitectura del procesador, previniendo corrupción de memoria. Finalmente, neutralicé un fallo de lógica secuencial donde el procesador sobrescribía silenciosamente una alarma térmica por no aislar los estados con un bloque else.*

### Día 5: Implementación Segura (`.c`)
* **Concepto asimilado:** *Mi estrategia de validación defensiva se basa en aplicar "Cláusulas de Guardia" (Early Returns) en la primera línea de ejecución. Comprobar si un puntero es NULL es el primer y más crítico escudo porque el procesador es ciego; si intenta leer o modificar la dirección de memoria 0x00000000, el microcontrolador sufre un colapso físico instantáneo (HardFault o Segmentation Fault). Esta barrera aborta la función de forma segura antes de tocar el silicio, asumiendo que cualquier dato entrante es radiactivo hasta demostrar lo contrario.*
* **Ejecución y Bloqueos:** *A nivel de compilación, me enfrenté a un error del sistema operativo (Permiso denegado) al usar -o /main, entendiendo que la barra / intenta escribir el binario en la raíz absoluta de Linux y no en la carpeta local. También choqué con el error implicit declaration of function 'printf', lo que me demostró que los archivos .c no heredan los #include del main; tuve que importar <stdio.h> explícitamente en el módulo. A nivel estructural, asimilé que sin el escudo NULL, cualquier intento de manipular los índices head o tail de un Ring Buffer sobre una memoria inexistente destruiría el sistema al instante.*

### Día 6: Pruebas de Estrés y Casos Límite (`main.c`)
* **Concepto asimilado:** *Mi arquitectura de software responde a vectores destructivos (como inyectar un puntero NULL o valores de lectura desbordados como 65535) bloqueando la operación en la primera línea de ejecución antes de tocar la memoria. Asimilé que la validación del código no se hace con impresiones visuales, sino construyendo un entorno de pruebas con <assert.h>. Esta macro exige matemáticamente el resultado correcto ante casos límite (fronteras de activación y cero absoluto); si el módulo falla en un solo dígito, la aserción "mata" el programa y evita que el bug pase al silicio.*
* **Ejecución y Bloqueos:** *Al compilar y ejecutar, no hubo bloqueos de aserción. Los casos límite de frontera (449 y 450) fueron evaluados como verdaderos de forma silenciosa por la máquina, y el ataque de estrés con el puntero inválido fue interceptado con éxito por la cláusula de guardia, demostrando que el módulo es seguro*

### Día 7: Auditoría y Cierre
**Hitos y Cicatrices de Compilación:**
*   **Mapeo de Memoria:** Comprobé visualmente (usando `%p` y el operador `&`) la inmensa distancia física que separa al *Data Segment* (variables globales) del *Stack* (memoria local destructible).
*   **Navegación Pura:** Abandoné la abstracción de los corchetes `[]` para usar aritmética de punteros `*(ptr + offset)`, obligando a mi mente a dar "saltos de memoria" tal como lo hace la ALU del procesador.
*   **Lógica Circular:** Implementé un mapeo matemático seguro (operador `%`) para reciclar arreglos estáticos de forma infinita, garantizando que un flujo continuo de datos de sensores nunca colapse la memoria disponible.
*   **Modularidad de Hardware:** Rompí el archivo monolítico y construí Contratos (`.h` protegidos por *include guards*) separados de las Fábricas (`.c`), asimilando que los archivos son completamente ciegos entre sí hasta que el compilador y el Linker los ensamblan.
*   **Filosofía Defensiva:** Asumí que cualquier dato entrante en una función es radiactivo. Las validaciones contra `NULL` en la primera línea de ejecución son ahora el estándar obligatorio para evitar colapsos catastróficos.
*   **Pruebas Destructivas:** Dejé de validar código "mirando" la terminal. Ahora construyo cámaras de tortura con `<assert.h>` que fuerzan el código hacia sus límites lógicos y matemáticos.