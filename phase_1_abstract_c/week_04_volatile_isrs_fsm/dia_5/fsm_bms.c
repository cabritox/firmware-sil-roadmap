#include <stdio.h>
#include <stdbool.h>

/* 1. Definicion de los Estados del Sistema */
typedef enum
{
    ST_REPOSO = 0,
    ST_CARGA,
    ST_ERROR,
    ST_MAX /* Etiqueta util para conocer el tamano exacto de la tabla */
} EstadoBMS_t;

/* 2. Firma comun para los punteros a funcion de la FSM.
   Reciben el evento y RETORNAN el siguiente estado al que transicionar. */
typedef EstadoBMS_t (*FsmHandler_t)(char evento);

/* 3. Declaracion de las funciones manejadoras de cada estado */
EstadoBMS_t estado_reposo(char evento);
EstadoBMS_t estado_carga(char evento);
EstadoBMS_t estado_error(char evento);

/* 4. LA TABLA DE DESPACHO DE LA FSM (Ejecucion O(1)) */
FsmHandler_t tabla_fsm[ST_MAX] = {
    estado_reposo, /* Indice 0 */
    estado_carga,  /* Indice 1 */
    estado_error   /* Indice 2 */
};

/* --- IMPLEMENTACION DE LOS ESTADOS --- */

EstadoBMS_t estado_reposo(char evento)
{
    if (evento == 'c')
    {
        printf("[BMS] Cargador conectado. Cerrando contactores...\n");
        return ST_CARGA; /* Transicionamos a CARGA */
    }
    if (evento == 'f')
    {
        printf("[BMS] ALERTA: Falla detectada en reposo. Abriendo circuito!\n");
        return ST_ERROR; /* Transicionamos a ERROR */
    }

    /* Si llega un evento 'd' o 'r', no tienen sentido en Reposo. Se ignoran. */
    return ST_REPOSO;
}

EstadoBMS_t estado_carga(char evento)
{
    /* TU MISION AQUI:
       1. Si llega 'd', imprimir que se desconecto y retornar ST_REPOSO.
       2. Si llega 'f', imprimir alerta de sobrecarga y retornar ST_ERROR.
       3. Cualquier otro evento, retornar ST_CARGA. */

    if (evento == 'd'){
        printf("[BMS] Cargador desconectado. Abriendo contactores...\n");
        return ST_REPOSO;
    }
    if (evento == 'f'){
        printf("[BMS] Alerta de sobrecarga. Abriendo circuito!\n");
        return ST_ERROR;
    }

    return ST_CARGA; // Placeholder
}

EstadoBMS_t estado_error(char evento)
{
    /* TU MISION AQUI:
       1. El BMS esta bloqueado. Ignora 'c' y 'd'.
       2. Si llega 'r' (reset), imprimir que se reinicia el sistema y retornar ST_REPOSO.
       3. Cualquier otro evento, retornar ST_ERROR. */
    if (evento == 'r'){
        printf("[BMS] Reiniciando sistema...\n");
        return ST_REPOSO;
    }

    printf("[BMS] Sistema bloqueado, reinicia para continuar...\n");
    return ST_ERROR; // Placeholder
}

/* --- BUCLE PRINCIPAL (El motor de la maquina) --- */
int main(void)
{
    EstadoBMS_t estado_actual = ST_REPOSO;
    char evento_teclado = '\0';

    printf("=== SIMULADOR DE BMS (FSM) ===\n");
    printf("Comandos: [c]onectar, [d]esconectar, [f]alla, [r]eset, [q]uit\n\n");

    /* Bucle infinito imitando el hardware */
    while (evento_teclado != 'q')
    {

        printf("Estado Actual: %d | Ingrese evento: ", estado_actual);
        scanf(" %c", &evento_teclado); /* El espacio inicial limpia el buffer del Enter */

        if (evento_teclado == 'q')
        {
            printf("Apagando sistema...\n");
            break;
        }

        /* MAGIA DE LA FSM:
           Ejecutamos el handler del estado actual pasandole el evento,
           y sobreescribimos el estado actual con el resultado. */
        estado_actual = tabla_fsm[estado_actual](evento_teclado);
    }

    return 0;
}