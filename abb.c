#include "abb.h"
#include "abb_aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct abb{
	abb_nodo_t* raiz;
	size_t cant;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t funcion_destruir;
};

struct abb_iter{
	const abb_t* arbol;
	abb_nodo_t* actual;
};

//************** PRIMITIVAS DEL ARBOL ****************
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* arbol = malloc( sizeof( abb_t ));
	if (! arbol ) return NULL;
	arbol->cant = 0;
	arbol->cmp = cmp;
	arbol->funcion_destruir = destruir_dato;
	arbol->raiz = NULL;
	return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	if (! arbol) return false;
	return abb_guardar_aux( arbol, arbol->raiz, clave, dato);
}

void *abb_borrar(abb_t *arbol, const char *clave){
	if (! arbol ) return NULL;
	return abb_borrar_aux(arbol, arbol->raiz, clave);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	abb_nodo_t* nodo = buscar_dato( arbol->raiz, clave, arbol->cmp);
	if (! nodo ) return NULL;
	return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	abb_nodo_t* nodo = buscar_dato( arbol->raiz, clave, arbol->cmp);
	return nodo != NULL;
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cant;
}
	
void abb_destruir(abb_t *arbol){
	abb_destruir_aux( arbol, arbol->raiz);
	free( arbol );
}
	
//************* ITERADOR INTERNO **********************

void abb_in_order_aux(abb_nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra){
	if (! nodo ) return;
	
	abb_in_order_aux( nodo->izq, visitar, extra );
	if ( visitar ){
		visitar( nodo->clave, nodo->dato, extra );
	}
	abb_in_order_aux( nodo->der, visitar, extra );
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	if (! arbol ) return;
	
	abb_in_order_aux(arbol->raiz, visitar,extra);
}

//**************** ITERRADOR EXTERNO ******************

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* iter = malloc( sizeof ( abb_iter_t ));
	if (! iter ) return NULL;
	iter->arbol = arbol;
	iter->actual = arbol->raiz; 
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	
	//wtf!
	if ( iter->actual == NULL) return false;
	if ( iter->actual->der != NULL )
		iter->actual = iter->actual->der;
	if ( iter->actual->der == NULL && iter->actual->izq != NULL)
		iter->actual = iter->actual->izq;
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	if ( iter->actual == NULL )  return NULL;
	return iter->actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return iter->actual == NULL;
}

void abb_iter_in_destruir(abb_iter_t* iter){
	free(iter);
}
