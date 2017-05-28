#include "abb.h"
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct abb_nodo{
	void* dato;
	char* clave;
	struct abb_nodo* padre;
	struct abb_nodo* izq;
	struct abb_nodo* der;
}typedef abb_nodo_t;

struct abb{
	abb_nodo_t* raiz;
	size_t cant;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t funcion_destruir;
};

struct abb_iter{
	abb_t* arbol; //Faltaba el arbol.
	pila_t* pila;
	abb_nodo_t* actual;
};

// ************* FUNCIONES AUXILIARES **************
abb_nodo_t* buscar_dato(abb_nodo_t* nodo, const char* clave, abb_comparar_clave_t cmp){
	if (! nodo ) return NULL;
	int comp = cmp(nodo->clave, clave);
	if ( comp == 0)
		return nodo;
	if ( comp > 0)
		return buscar_dato( nodo->izq, clave , cmp);
	if ( comp < 0)
		return buscar_dato( nodo->der, clave, cmp);

	return NULL;
}

abb_nodo_t* buscar_mayor(abb_nodo_t* nodo){
	if ( nodo == NULL ) 
		return NULL; //El nodo no existe, como obtienes el padre? ~GIUS
	// busco mayor siempre a la derecha
	while ( nodo->der != NULL )
		nodo = nodo->der;
	return nodo;
}

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

void *abb_obtener(const abb_t *arbol, const char *clave){
	if (! arbol ) return NULL;
	abb_nodo_t* nodo = buscar_dato( arbol->raiz, clave, arbol->cmp);
	if (! nodo ) return NULL;
	return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	if (! arbol ) return NULL;
	abb_nodo_t* nodo = buscar_dato( arbol->raiz, clave, arbol->cmp);
	return nodo != NULL;
}

size_t abb_cantidad(abb_t *arbol){
	if (! arbol ) return 0;
	return arbol->cant;
}
// ************** GUARDAR ***************

abb_nodo_t* crear_nodo( const char *clave, void *dato, abb_nodo_t* padre){
	abb_nodo_t* nodo = malloc( sizeof( abb_nodo_t ));
	if (! nodo ) return NULL;
	
	char* clave_aux = malloc( sizeof(char) * (strlen( clave ) + 1));
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

bool abb_guardar_aux(abb_t* arbol, abb_nodo_t *nodo, const char *clave, void *dato, abb_nodo_t*  padre){
	if ( nodo == NULL ){
		abb_nodo_t* nuevo = crear_nodo( clave, dato, padre);
		if (! nuevo ) 
			return false;
		
		if ( padre != NULL){
			int c = arbol->cmp(clave, padre->clave);
			if (c < 0) padre->izq = nuevo;
			else padre->der = nuevo;
		}
		//nodo = nuevo; Esto no nos ayuda. Nodo es una variable que guarda el NULL y despues no la volvemos a ver
		arbol->cant++;
		return true;
	}
	int comp = arbol->cmp(nodo->clave, clave);
	if ( comp == 0){ 
		if ( arbol->funcion_destruir ){
			arbol->funcion_destruir(nodo->dato);
		}
		nodo->dato = dato;
		return true;
	}
	if ( comp > 0){ 
		return abb_guardar_aux(arbol, nodo->izq, clave, dato, nodo); 
	}
	if ( comp < 0){
		return abb_guardar_aux(arbol, nodo->der, clave, dato, nodo);
	}
	return false;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	if (! arbol) return false;
	
	/* Esto no hara falta, cumple ya la condicion inicial if (!nodo), junto a padre == NULL.
	if (arbol->cant == 0){
		abb_nodo_t* nuevo = crear_nodo(clave,dato,arbol->raiz);
		arbol->raiz = nuevo;
		arbol->cant++;
		return true;
	}
	*/
	return abb_guardar_aux( arbol, arbol->raiz, clave, dato, NULL);
}

// ******************** BORRAR ***********
void* destruir_nodo(abb_nodo_t* nodo){
	if (! nodo ) return NULL;
	void* dato = nodo->dato;
	free( nodo->clave );
	free( nodo );
	return dato;
}

void* borrar_sin_hijos(abb_t* arbol, abb_nodo_t* nodo, abb_nodo_t* padre){
	if ( padre == NULL )
		arbol->raiz = NULL;
	else if ( padre->izq == nodo)
		padre->izq = NULL;
	else if ( padre->der == nodo )
		padre->der = NULL; 
	arbol->cant--;
	return destruir_nodo(nodo);
}

void* borrar_un_hijo(abb_t* arbol, abb_nodo_t* nodo, abb_nodo_t* padre){
	if ( nodo->izq && (!nodo->der)){
		if (! padre ) arbol->raiz = nodo->izq;
		else if ( padre->izq == nodo) 
			nodo->padre->izq = nodo->izq;

		else if ( padre->der == nodo )
			padre->der = nodo->izq;

		nodo->izq->padre = nodo->padre;
		arbol->cant--;
		return destruir_nodo(nodo);
	}
	if ( nodo->der && (!nodo->izq)){
		if (! padre ) arbol->raiz = nodo->der;
		else if ( padre->izq == nodo )  
			padre->izq = nodo->der;

		else if ( padre->der == nodo )
			padre->der = nodo->der; 

		nodo->der->padre = nodo->padre;
		arbol->cant--;
		return destruir_nodo(nodo);
	}
	return NULL;
}

abb_nodo_t* borrar_dos_hijos(abb_t* arbol, abb_nodo_t* nodo, abb_nodo_t* padre){
	abb_nodo_t* nodo_mayor = buscar_mayor( nodo->izq );
	//
	if (! padre) arbol->raiz = nodo_mayor;

	if ( nodo_mayor->izq ) nodo_mayor->padre->der = nodo_mayor->izq;
	if (padre->izq == nodo) padre->izq = nodo_mayor;
	if (padre->der == nodo) padre->der = nodo_mayor;

	nodo_mayor->izq = nodo->izq;
	nodo_mayor->der = nodo->der;

	arbol->cant--;
	return destruir_nodo(nodo);
}

//Este es una alternativa para borrar que cubre todos los nodos, tengan dos hijos, uno o ninguno. ~GIUS
void* borrar_alt(abb_t* arbol, abb_nodo_t* nodo, abb_nodo_t* padre){
	
	abb_nodo_t* nodo_mayor;
	if (!nodo->izq) nodo_mayor = nodo->der;
	else nodo_mayor = buscar_mayor( nodo->izq );
	
	if (! padre) arbol->raiz = nodo_mayor;
	else {
		int c = arbol->cmp(nodo->clave,padre->clave);
		if (c < 0) padre->izq = nodo_mayor;
		else padre->der = nodo_mayor;
	}
	
	if (nodo_mayor ) {
		//Estas condiciones aplican solo cuando nodo_mayor viene del fondo de la izq
		if (nodo_mayor->padre != nodo) {
			nodo_mayor->padre->der = nodo_mayor->izq;
			nodo_mayor->izq = nodo->izq;
		}
		//Y esta cuando nodo_mayor viene de la izq y tiene razon para no tener der.
		if (nodo_mayor != nodo->der) nodo_mayor->der = nodo->der;
		nodo_mayor->padre = padre;
	}
	
	arbol->cant--;
	return destruir_nodo(nodo);
}


void* abb_borrar_aux(abb_t* arbol, abb_nodo_t *nodo, abb_nodo_t* padre, const char *clave){
	int comp = arbol->cmp( nodo->clave, clave );
	if ( comp > 0 )
		abb_borrar_aux( arbol, nodo->izq, nodo, clave );
	if ( comp < 0 )
		abb_borrar_aux( arbol, nodo->der, nodo, clave);

	if ( comp == 0 ){
		/*
		if ( !nodo->izq && !nodo->der )
			return borrar_sin_hijos(arbol, nodo, padre);
		
		if ( (nodo->izq && !nodo->der ) || (nodo->der && !nodo->izq ) )
			return borrar_un_hijo(arbol, nodo, padre);

		if ( nodo->izq && nodo->der )
			return borrar_dos_hijos(arbol, nodo, padre);
		*/
		return borrar_alt(arbol, nodo, padre);
	}
	return NULL;
}

//Si el padre viene en la estructura del nodo, es necesario llamarlo aparte? ~GIUS

void *abb_borrar(abb_t *arbol, const char *clave){
	if (! arbol ) return NULL;
	if (! abb_pertenece(arbol, clave)) return NULL;
	return abb_borrar_aux(arbol, arbol->raiz, NULL, clave);
}

// ***************** DESTRUIR **************
void abb_destruir_aux(abb_t* arbol, abb_nodo_t* nodo){
	if (! nodo) return;

	abb_destruir_aux( arbol, nodo->izq );
	abb_destruir_aux( arbol, nodo->der );
	void* dato = destruir_nodo(nodo);
	if ( arbol->funcion_destruir ){
		arbol->funcion_destruir( dato );
	}
}

void abb_destruir(abb_t *arbol){
	abb_destruir_aux( arbol, arbol->raiz);
	free( arbol );
}
	
//************* ITERADOR INTERNO **********************

bool abb_in_order_aux(abb_nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra){
	if (! nodo ) return true; //Sigue iterando
	if (!abb_in_order_aux( nodo->izq, visitar, extra )) return false; //y la iteracion termina ahi.
	if (!visitar( nodo->clave, nodo->dato, extra )) return false;
	return abb_in_order_aux( nodo->der, visitar, extra );
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	if (! arbol ) return;
	abb_in_order_aux(arbol->raiz, visitar,extra);
}

//**************** ITERRADOR EXTERNO ******************

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	if (! arbol) return NULL;
	abb_iter_t* iter = malloc( sizeof ( abb_iter_t ));
	if (! iter ) return NULL;
	iter->pila = pila_crear();
	abb_nodo_t* nodo = arbol->raiz;
	while (nodo && nodo->izq){
		pila_apilar(iter->pila, nodo);
		nodo = nodo->izq;
	}
	iter->actual = nodo;
	iter->arbol = arbol;
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if (abb_iter_in_al_final(iter)) return false;
	if (! iter->actual->der) 
		iter->actual = pila_desapilar(iter->pila);
	else{
		iter->actual = iter->actual->der;
		while (iter->actual->izq){
			pila_apilar(iter->pila, iter->actual);
			iter->actual = iter->actual->izq;
		}
	}
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	if (! iter->actual )  return NULL;
	return iter->actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return iter->actual == NULL;
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
}
