#include <stdio.h>

/* 1. Funciones de hardware especificas.
      Todas comparten la misma "firma": retornan void y no reciben parametros (void). */
void valvula_abrir(void)
{
    printf("[Hardware] Valvula ABIERTA.\n");
}

void valvula_cerrar(void)
{
    printf("[Hardware] Valvula CERRADA.\n");
}

void valvula_purgar(void)
{
    printf("[Hardware] Valvula PURGANDO.\n");
}

/* 2. Funcion de Alto Nivel (Usa un Callback)
      Recibe como parametro UN PUNTERO a cualquier funcion que cumpla la firma. */
void ejecutar_accion_segura(void (*accion_ptr)(void))
{
    printf(">> Verificando presiones del sistema...\n");

    /* Ejecutamos la funcion que nos pasaron */
    accion_ptr();

    printf(">> Accion completada con exito.\n\n");
}

int main(void)
{
    /* Declaracion de un puntero a funcion.
       Sintaxis: tipo_retorno (*nombre_puntero)(tipos_parametros); */
    void (*puntero_hardware)(void);

    printf("--- EJECUCION DINAMICA DIRECTA ---\n");

    /* Apuntamos a la primera funcion y la ejecutamos */
    puntero_hardware = &valvula_abrir;
    puntero_hardware();

    /* Nota: En C, el '&' es opcional al apuntar a funciones.
       El nombre de la funcion ya es su direccion de memoria. */
    puntero_hardware = valvula_cerrar;
    puntero_hardware();

    printf("\n--- EJECUCION POR CALLBACKS ---\n");

    /* Inyectamos el comportamiento directamente en otra funcion */
    ejecutar_accion_segura(valvula_abrir);
    ejecutar_accion_segura(valvula_cerrar);
    ejecutar_accion_segura(valvula_purgar);

    return 0;
}