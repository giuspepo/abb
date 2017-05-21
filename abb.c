#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct abb_nodo{
	void* dato;
	char* clave;
	struct abb_nodo padre;
	struct abb_nodo izq;
	struct abb_nodo der;
}typedef abb_nodo_t;

struct abb{
	abb_nodo_t* raiz;
	size_t cant;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t funcion_destruir;
};

struct abb_iter{
	const abb_t* arbol;
	abb_nodo_t* anterior;
	abb_nodo_t* actual;
};

// Crea una hoja sin hijos, con referencia al padre.
// Se guarda en el el dato y una copia de la clave
abb_nodo_t* crear_nodo( const char *clave, void *dato, abb_nodo_t* padre){
	abb_nodo_t* nodo = malloc( sizeof( abb_nodo_t ));
	if (! nodo ) return NULL;
	
	char* clave_aux = malloc( strlen( clave ) + 1);
	if ( clave_aux == NULL ){
		free( nodo );
		return NULL;
	}
	strcpy(clave_aux, clave);

	nodo->dato = dato;
	nodo->clave = clave_aux;
	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->padre = padre;
	return nodo;
}

// Devuelve nodo arbol de la clave buscada
abb_nodo_t* buscar_dato(abb_nodo_t* nodo, const char* clave, abb_comparar_clave_t cmp){
	if (! nodo ) return NULL;
	if ( cmp( nodo->clave, clave ) == 0)
		return nodo;

	if ( cmp( nodo->clave, clave ) < 0)
		return buscar_dato( nodo->izq, clave , cmp);

	if ( cmp( nodo->clave, clave ) < 0)
		return buscar_dato( nodo->der, clave, cmp);
	return NULL;
}

// Devuelve nodo arbol de mayor valor
abb_nodo_t* buscar_mayor(abb_nodo_t* nodo, abb_comparar_clave_t cmp){
	if ( nodo == NULL ) 
		return;
	// busco mayor siempre a la derecha
	if ( cmp ( nodo->der->clave, nodo->clave) < 0)
		return nodo;
	buscar_mayor( nodo->der, cmp);
}

//************** PRIMITIVAS DEL ARBOL ****************
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* arbol = malloc( 	abb_t* arbol = malloc( sizeof( abb_t )));
	if (! arbol ) return NULL;
	arbol->cant = 0;
	arbol->cmp = cmp;
	arbol->funcion_destruir = destruir_dato;
	arbol->raiz = NULL;
	return arbol;
}

bool abb_guardar_aux(abb_t* arbol, abb_nodo_t *nodo, const char *clave, void *dato){
	
	if ( nodo == NULL ){
		abb_nodo_t* nuevo = crear_nodo( clave, dato, nodo->padre); //llega al final de hojas, el padre es el padre de null??
		if (! nuevo ) 
			return false;
		if (! arbol->raiz ) arbol->raiz = nuevo;
		nodo = nuevo;
		arbol->cant++;
		return true;
	}

	if (arbol->cmp(nodo->clave, clave) == 0){
		if (arbol->destruir_dato != NULL){
			arbol->destruir_dato(nodo->dato);
		}
		nodo->dato = dato;
		return true;
	}
	if (arbol->cmp(nodo->clave, clave) < 0)
		return abb_guardar_aux(arbol, nodo->izq, clave, dato);
	if (arbol->cmp(nodo->clave, clave) > 0)
		return abb_guardar_aux(arbol, nodo->der, clave, dato);	
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	if (! arbol) return false;
	return abb_guardar_aux( arbol, arbol->raiz, clave, dato);
}

// ****************** FUNCIONES PARA BORRAR ****************
abb_nodo_t* borrar_sin_hijos(abb_nodo_t* nodo){
	if (nodo->padre->izq == nodo)
		padre->izq = NULL;
	if (nodo->padre->der == nodo)
		padre->der = NULL;

	return nodo;
}

abb_nodo_t* borrar_un_hijo(abb_nodo_t* nodo){
	if ( nodo->izq != NULL){
		if ( nodo->padre->izq == nodo ) // 1. si padre esta en izquiera le asigno hijo de nodo izquiero
			nodo->padre->izq = nodo->izq;

		nodo->padre->der = nodo->izq; // 2. padre en la derecha le asigno hijo izquierdo
		nodo->izq->padre = nodo->padre;
	}
	if ( nodo->der != NULL){
		if ( nodo->padre->izq == nodo ) //1. 
			nodo->padre->izq = nodo->der;

		nodo->padre->der = nodo->der; //2.
		nodo->der->padre = nodo->padre;
	}
	return nodo;
}

void swap (abb_nodo_t* nodo_borrar, abb_nodo_t* nodo_mayor) {
	abb_nodo_t* guardar = nodo_borrar;
	*nodo_borrar = *nodo_mayor;
	*nodo_mayor = guardar;
}

abb_nodo_t* borrar_dos_hijos( abb_noto_t* nodo, abb_comparar_clave_t cmp){
	// buscar mayor de la izquierda
	abb_nodo_t* nodo_mayor = buscar_mayor( nodo->izq , cmp);
	// swap de nodo con nodo mayor
	swap( nodo, nodo_mayor)
	// borrar nodo simple con un hijo
	abb_nodo_t* nodo_borrar = borrar_sin_hijos(nodo);
	return nodo_borrar;
}

void* abb_borrar_aux(abb_t* arbol, abb_nodo_t *nodo, const char *clave){

	if ( nodo != NULL ){
		if ( arbol->cmp( nodo->clave, clave ) < 0 )
			abb_borrar_aux( arbol, nodo->izq, clave );

		if ( arbol->cmp( nodo->clave, clave ) > 0 )
			abb_borrar_aux( arbol, nodo->der, clave);

		if ( arbol->cmp( nodo->clave, clave ) == 0 ){
			
			// CASO SIN NINGUN HIJO
			if ( nodo->izq == NULL && nodo->der == NULL )
				abb_nodo_t* nodo_borrar = borrar_sin_hijos(nodo);
			
			// CASO CON UN HIJO
			//if ( (nodo->izq != NULL && !nodo->der ) || (nodo->der != NULL && !nodo->izq ) ){
			if ( nodo->der != NULL || nodo->izq != NULL )
				abb_nodo_t* nodo_borrar = borrar_un_hijo(nodo);
			
			// CASO CON DOS HIJOS
			if ( nodo->izq != NULL && nodo->der != NULL ){
				abb_nodo_t* nodo_borrar =borrar_dos_hijos(nodo, arbol->cmp);
			}
			void dato = nodo_borrar->dato;
			abb_destruir_aux(nodo_borrar, arbol->destruir_dato);
			arbol->cant--;
			return dato;
		}
	}
	return NULL;
}

void *abb_borrar(abb_t *arbol, const char *clave){
	
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

// Funcion auxiliar para destruir arbol POSTORDER
void abb_destruir_aux(abb_nodo_t* nodo, abb_destruir_dato_t destruir_dato){
	if (! nodo )
		return;

	// Postorder
	abb_destruir( nodo->izq );
	abb_destruir( nodo->der );
	if ( nodo->funcion_destruir ){
		funcion_destruir ( nodo->dato );
	}
	free( nodo->clave );
	free( nodo );
}
	
void abb_destruir(abb_t *arbol){
	if (! arbol ){
		return;
	}
	abb_destruir_aux( arbol->raiz, arbol->funcion_destruir );
	free( arbol );
}
	
//************* ITERADOR INTERNO **********************

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	if (! arbol ) return;
	
	abb_in_order( arbol->raiz->izq, visitar, extra );
	if ( visitar ){
		visitar( arbol->raiz->clave, arbol->nodo->dato, extra );
	}
	abb_in_order( arbol->raiz->der, visitar, extra );
}

//**************** ITERRADOR EXTERNO ******************

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* iter = malloc( sizeof ( abb_iter_t ));
	if (! iter ) return NULL;
	iter->arbol = arbol;
	iter->anterior = NULL;
	iter->actual = arbol->raiz; 
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
