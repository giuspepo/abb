#include "abb.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void prueba_crear_abb_vacio(){
    abb_t* arbol = abb_crear( NULL, NULL );

    print_test("Prueba abb crear hash vacio", arbol ) ;
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(arbol) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(arbol, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(arbol, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(arbol, "A"));

    abb_destruir(arbol);
}

void prueba_iterar_abb_vacio(){
    abb_t* arbol = abb_crear( NULL, NULL );

    abb_iter_t* iter = abb_iter_crear(arbol);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_ver_actual(iter));

    abb_iter_destruir(iter);
    abb_destruir(arbol);
}

void pruebas_abb_alumno(){
	prueba_crear_abb_vacio();
	prueba_iterar_abb_vacio();


}