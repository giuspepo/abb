#include "abb.h"
//#include "abb_aux.h"
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

    abb_iter_t* iter = abb_iter_in_crear(arbol);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(arbol);
}

int comparar_clave(const char* clave1, const char* clave2){
	if (clave1 > clave2) return -1;
	if (clave1 < clave2) return 1;
	return 0;
}

void prueba_abb_insertar(){
	abb_t* arbol = abb_crear( comparar_clave, NULL );

	char *clave1 = "saludo", *valor1 = "hola";
	char *clave2 = "nombre", *valor2 = "apellido";
	char *clave3 = "animal", *valor3 = "perro";
	char *clave4 = "dia", *valor4 = "lunes";

	/* Inserta 1 valor y luego lo borra */
	print_test("Prueba insertar clave1", abb_guardar(arbol, clave1, valor1));
	print_test("Prueba la cantidad de elementos es 1", abb_cantidad(arbol) == 1);
	print_test("Prueba obtener clave1 es valor1", abb_obtener(arbol, clave1) == valor1);
	print_test("Prueba pertenece clave1, es true", abb_pertenece(arbol, clave1));
	print_test("Prueba borrar clave1, es valor1", abb_borrar(arbol, clave1) == valor1);
	print_test("Prueba la cantidad de elementos es 0", abb_cantidad(arbol) == 0);

	print_test("Prueba insertar clave2", abb_guardar(arbol, clave2, valor2));
	print_test("Prueba la cantidad de elementos es 1", abb_cantidad(arbol) == 1);
	print_test("Prueba obtener clave2 es valor2", abb_obtener(arbol, clave2) == valor2);
	print_test("Prueba pertenece clave2, es true", abb_pertenece(arbol, clave2));

	print_test("Prueba insertar clave3", abb_guardar(arbol, clave3, valor3));
	print_test("Prueba la cantidad de elementos es 2", abb_cantidad(arbol) == 2);
	print_test("Prueba obtener clave3 es valor3", abb_obtener(arbol, clave3) == valor3);
	print_test("Prueba pertenece clave3, es true", abb_pertenece(arbol, clave3));

	print_test("Prueba insertar clave4", abb_guardar(arbol, clave4, valor4));
	print_test("Prueba la cantidad de elementos es 3", abb_cantidad(arbol) == 3);
	print_test("Prueba obtener clave4 es valor4", abb_obtener(arbol, clave4) == valor4);
	print_test("Prueba pertenece clave4, es true", abb_pertenece(arbol, clave4));

	print_test("Prueba borrar clave2, es valor2", abb_borrar(arbol, clave2) == valor2);
	print_test("Prueba la cantidad de elementos es 2", abb_cantidad(arbol) == 2);

	print_test("Prueba borrar clave3, es valor3", abb_borrar(arbol, clave3) == valor3);
	print_test("Prueba la cantidad de elementos es 1", abb_cantidad(arbol) == 1);

	print_test("Prueba borrar clave4, es valor4", abb_borrar(arbol, clave4) == valor4);
	print_test("Prueba la cantidad de elementos es 0", abb_cantidad(arbol) == 0);

	abb_destruir(arbol);
}

void prueba_abb_reemplazar(){ 
    abb_t* arbol = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("Prueba insertar clave1", abb_guardar(arbol, clave1, valor1a));
    print_test("Prueba obtener clave1 es valor1a", abb_obtener(arbol, clave1) == valor1a);
    print_test("Prueba pertenece clave1 es valor1a", abb_pertenece(arbol, clave1));
    print_test("Prueba insertar clave2", abb_guardar(arbol, clave2, valor2a));

    print_test("Prueba pertenece clave2 es valor2a", abb_pertenece(arbol, clave2));
    print_test("Prueba obtener clave2 es valor2a", abb_obtener(arbol, clave2) == valor2a);
    print_test("Prueba hash la cantidad de elementos es 2", abb_cantidad() == 2);

    print_test("Prueba insertar clave1 con otro valor", abb_guardar(arbol, clave1, valor1b));
    print_test("Prueba pertenece clave1 es valor1b", abb_pertenece(arbol, clave1));
    print_test("Prueba obtener clave1 es valor1b", abb_obtener(arbol, clave1) == valor1b);

    print_test("Prueba insertar clave2 con otro valor", abb_guardar(arbol, clave2, valor2b));
    print_test("Prueba pertenece clave2 es valor2b", abb_pertenece(arbol, clave2));
    print_test("Prueba obtener clave2 es valor2b", abb_obtener(arbol, clave2) == valor2b);
    print_test("Prueba la cantidad de elementos es 2", abb_cantidad(arbol) == 2);

    abb_destruir(arbol);
}


void pruebas_abb_alumno(){
	prueba_crear_abb_vacio();
	prueba_iterar_abb_vacio();
	prueba_abb_insertar();
	prueba_abb_reemplazar();
}
