#ifndef ABB_AUX_H
#define ABB_AUX_H

#include <stdbool.h>
#include <stddef.h>

struct abb_nodo{
	void* dato;
	char* clave;
	struct abb_nodo* padre;
	struct abb_nodo* izq;
	struct abb_nodo* der;
}typedef abb_nodo_t;

// ************* FUNCIONES AUXILIARES **************

// Crea una hoja sin hijos, con referencia al padre.
// Se guarda en el el dato y una copia de la clave
abb_nodo_t* crear_nodo( const char *clave, void *dato, abb_nodo_t* padre);

// Devuelve nodo_abb de la clave buscada
abb_nodo_t* buscar_dato(abb_nodo_t* nodo, const char* clave, abb_comparar_clave_t cmp);

// Devuelve nodo_abb de mayor valor
abb_nodo_t* buscar_mayor(abb_nodo_t* nodo, abb_comparar_clave_t cmp);

// Funcion auxiliar para guardar el par clave, valor en un nodo_abb
bool abb_guardar_aux(abb_t* arbol, abb_nodo_t *nodo, const char *clave, void *dato);

// Borra un nodo simple, que no tiene hijos
abb_nodo_t* borrar_sin_hijos(abb_nodo_t* nodo);

// Borra el nodo si contiene un solo hijo, izquierdo o derecho
abb_nodo_t* borrar_un_hijo(abb_nodo_t* nodo);

// Invierte la posicion de dos nodos
void swap (abb_nodo_t* nodo_borrar, abb_nodo_t* nodo_mayor);

// Borra el nodo si tiene hijo izquierdo e hijo derecho
abb_nodo_t* borrar_dos_hijos( abb_nodo_t* nodo, abb_comparar_clave_t cmp);

// Funcion auxiliar para destruir arbol POSTORDER
void abb_destruir_aux(abb_t* arbol, abb_nodo_t* nodo);

// Funcion auxiliar para borrar un valor que contiene un nodo_abb
void* abb_borrar_aux(abb_t* arbol, abb_nodo_t *nodo, const char *clave);

// Funcion auxiliar para iterar en order los nodos del arbol
void abb_in_order_aux(abb_nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra);

#endif  // ABB_AUX_H
