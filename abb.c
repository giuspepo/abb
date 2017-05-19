#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct abb{
	void* dato;
	char* clave;
	size_t cant;
	struct abb izq;
	struct abb der;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t funcion_destruir;
};

struct abb_iter{
	const abb_t* arbol;
	abb_t* anterior;
	abb_t* actual;
};

// Crea una hoja sin hijos
abb_t* crear_hoja(const char *clave, void *dato){
	abb_t* hoja = malloc( 	abb_t* arbol = malloc( sizeof( abb_t ));
	if (! hoja ) return NULL;
	hoja->dato = dato;
	hoja->clave = clave;
	hoja->izq = NULL;
	hoja->der = NULL;
	return hoja;
}

// Devuelve arbol de la clave buscada
abb_t* buscar_dato(abb_t* arbol, const char* clave, abb_comparar_clave_t cmp){
	if (! arbol ) return NULL;
	if ( cmp( arbol->clave, clave ) == 0)
		return arbol;
	if ( cmp( arbol->clave, clave ) < 0)
		return abb_pertenece( arbol->izq, clave , cmp);
	if ( cmp( arbol->clave, clave ) < 0)
		return abb_pertenece( arbol->der, clave, cmp);
	return NULL;
}

//************** PRIMITIVAS DEL ARBOL ****************
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* arbol = malloc( 	abb_t* arbol = malloc( sizeof( abb_t ));
	if (! arbol ) return NULL;
	arbol->cant = 0
	arbol->cmp = cmp;
	arbol->funcion_destruir = destruir_dato;
	arbol->izq = NULL;
	arbol->der =  NULL;
	return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	
}

void *abb_borrar(abb_t *arbol, const char *clave){
	
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	abb_t* hoja = buscar_dato( arbol, clave, arbol->cmp);
	if (! hoja ) return NULL;
	return hoja->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	abb_t* hoja = buscar_dato( arbol, clave, arbol->cmp);
	return hoja != NULL;
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cant;
}

// Funcion auxiliar para destruir arbol POSTORDER
void abb_destruir_aux(abb_t* arbol, abb_destruir_dato_t destruir_dato){
	if (! arbol){
		return;
	}
	// Postorder
	abb_destruir( arbol->izq );
	abb_destruir( arbol->der );
	if ( arbol->funcion_destruir ){
		funcion_destruir ( arbol->dato );
	}
	free( arbol->clave );
	free( arbol );
}
	
void abb_destruir(abb_t *arbol){
	if (! arbol){
		return;
	}
	abb_destruir_aux( arbol, arbol->funcion_destruir );
	free(arbol);
}
	
//************* ITERADOR INTERNO **********************

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	if (! arbol ) return;
	
	abb_in_order(arbol->izq, visitar, extra );
	if ( visitar ){
		visitar( arbol->clave, arbol->dato, extra );
	}
	abb_in_order(arbol->der, visitar, extra );
}

//**************** ITERRADOR EXTERNO ******************

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* iter = malloc( sizeof ( abb_iter_t ));
	if (! iter ) return NULL;
	iter->arbol = arbol;
	iter->anterior = NULL;
	iter->actual = arbol; //arbol->clave??
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	if ( iter->actual == NULL )  return NULL;
	return iter->actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return iter->actual != NULL;
}

void abb_iter_in_destruir(abb_iter_t* iter){
	free(iter)
}
