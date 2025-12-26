#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <iostream>
#include <string>

using namespace std;

//DEFINICIÓN DE ESTRUCTURAS (DATOS)


enum TipoNodo {
    ARCHIVO,
    CARPETA
};

// Avisamos que existen estas estructuras para poder usarlas en los punteros
struct Nodo;
struct ListaEnlazada;

// Lo principal: puede ser archivo o carpeta
struct Nodo {
    string nombre;
    TipoNodo tipo;
    string contenido;       // Solo para archivos
    Nodo* padre;            // Para saber quién es la carpeta superior
    ListaEnlazada* hijos;   // Lista de lo que tiene adentro (si es carpeta)
};

// El nodo interno de la lista (el que conecta con los hijos del arbol)
struct NodoLista {
    Nodo* datos;            // El archivo/carpeta real
    NodoLista* siguiente;   // El siguiente hermano
};

// La lista en sí misma (solo guarda el inicio)
struct ListaEnlazada {
    NodoLista* cabeza;
};


// PROTOTIPOS DE FUNCIONES (MENÚ)


// Inicialización
void inicializarLista(ListaEnlazada* lista);
Nodo* crearNodo(string nombre, TipoNodo tipo, Nodo* padre);

// Operaciones básicas de la estructura
void insertarEnLista(ListaEnlazada* lista, Nodo* nuevoHijo);

// Auxiliar para ver si funciona (luego borraremos esto)
void imprimirEstructura(Nodo* nodoActual, int nivel); 


// IMPLEMENTACIÓN (LÓGICA)


inline void inicializarLista(ListaEnlazada* lista) {
    lista->cabeza = nullptr;
}

// Función constructora manual
inline Nodo* crearNodo(string nombre, TipoNodo tipo, Nodo* padre) {
    Nodo* nuevo = new Nodo;
    nuevo->nombre = nombre;
    nuevo->tipo = tipo;
    nuevo->padre = padre;
    nuevo->contenido = "";

    // Si es carpeta, le creamos su lista de hijos vacía
    if (tipo == CARPETA) {
        nuevo->hijos = new ListaEnlazada;
        inicializarLista(nuevo->hijos);
    } else {
        nuevo->hijos = nullptr;
    }
    return nuevo;
}

// Agrega un nodo al final de la lista de hijos
inline void insertarEnLista(ListaEnlazada* lista, Nodo* nuevoHijo) {
    NodoLista* nuevoVagon = new NodoLista;
    nuevoVagon->datos = nuevoHijo;
    nuevoVagon->siguiente = nullptr;

    if (lista->cabeza == nullptr) {
        lista->cabeza = nuevoVagon;
    } else {
        NodoLista* aux = lista->cabeza;
        while (aux->siguiente != nullptr) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevoVagon;
    }
}

// Función recursiva simple para verificar visualmente que el árbol funciona
inline void imprimirEstructura(Nodo* nodoActual, int nivel) {
    // Indentación según el nivel
    for(int i = 0; i < nivel; i++) cout << "  ";
    
    if (nodoActual->tipo == CARPETA) {
        cout << "[D] " << nodoActual->nombre << endl;
        // Recorrer hijos
        if (nodoActual->hijos != nullptr) {
            NodoLista* aux = nodoActual->hijos->cabeza;
            while (aux != nullptr) {
                imprimirEstructura(aux->datos, nivel + 1);
                aux = aux->siguiente;
            }
        }
    } else {
        cout << "- " << nodoActual->nombre << endl;
    }
}

#endif