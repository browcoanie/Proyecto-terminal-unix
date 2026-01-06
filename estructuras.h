#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <iostream>
#include <string>

using namespace std;

// ==========================================
// 1. DEFINICIÓN DE ESTRUCTURAS (DATOS)
// ==========================================

enum TipoNodo {
    ARCHIVO,
    CARPETA
};

// Avisamos que existen estas estructuras
struct Nodo;
struct ListaEnlazada;

// El Nodo principal
struct Nodo {
    string nombre;
    TipoNodo tipo;
    string contenido;       // Solo para archivos
    Nodo* padre;            // Para saber quién es la carpeta superior
    ListaEnlazada* hijos;   // Lista de lo que tiene adentro (si es carpeta)
};

// El nodo interno de la lista
struct NodoLista {
    Nodo* datos;            // El archivo/carpeta real
    NodoLista* siguiente;   // El siguiente hermano
};

// La lista en sí misma
struct ListaEnlazada {
    NodoLista* cabeza;
};

// ==========================================
// 2. PROTOTIPOS DE FUNCIONES
// ==========================================

void inicializarLista(ListaEnlazada* lista);
Nodo* crearNodo(string nombre, TipoNodo tipo, Nodo* padre);
void insertarEnLista(ListaEnlazada* lista, Nodo* nuevoHijo);
void imprimirEstructura(Nodo* nodoActual, int nivel); 
Nodo* buscarEnLista(ListaEnlazada* lista, string nombreBuscado);
Nodo* desconectarNodo(ListaEnlazada* lista, string nombreNodo);

// ==========================================
// 3. IMPLEMENTACIÓN (LÓGICA)
// ==========================================

inline void inicializarLista(ListaEnlazada* lista) {
    lista->cabeza = nullptr;
}

inline Nodo* crearNodo(string nombre, TipoNodo tipo, Nodo* padre) {
    Nodo* nuevo = new Nodo;
    nuevo->nombre = nombre;
    nuevo->tipo = tipo;
    nuevo->padre = padre;
    nuevo->contenido = "";

    if (tipo == CARPETA) {
        nuevo->hijos = new ListaEnlazada;
        inicializarLista(nuevo->hijos);
    } else {
        nuevo->hijos = nullptr;
    }
    return nuevo;
}

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

inline void imprimirEstructura(Nodo* nodoActual, int nivel) {
    for(int i = 0; i < nivel; i++) cout << "  ";
    
    if (nodoActual->tipo == CARPETA) {
        cout << "[D] " << nodoActual->nombre << endl;
        if (nodoActual->hijos != nullptr) {
            NodoLista* aux = nodoActual->hijos->cabeza;
            while (aux != nullptr) {
                imprimirEstructura(aux->datos, nivel + 1);
                aux = aux->siguiente;
            }
        }
    } else {
        cout << "- " << nodoActual->nombre;
        if (!nodoActual->contenido.empty()) {
            cout << " (" << nodoActual->contenido.length() << " bytes)";
        }
        cout << endl;
    }
}

inline Nodo* buscarEnLista(ListaEnlazada* lista, string nombreBuscado) {
    if (lista == nullptr || lista->cabeza == nullptr) return nullptr;

    NodoLista* aux = lista->cabeza;
    while (aux != nullptr) {
        if (aux->datos->nombre == nombreBuscado) {
            return aux->datos;
        }
        aux = aux->siguiente;
    }
    return nullptr;
}

inline Nodo* desconectarNodo(ListaEnlazada* lista, string nombreNodo) {
    if (lista == nullptr || lista->cabeza == nullptr) {
        cout << "[DEBUG] Lista vacía o nula" << endl;
        return nullptr;
    }

    NodoLista* actual = lista->cabeza;
    NodoLista* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->datos->nombre == nombreNodo) {
            // Guardar el puntero al Nodo ANTES de eliminar el NodoLista
            Nodo* nodoRescatado = actual->datos;
            
            
            
            // Desconectar de la lista
            if (anterior == nullptr) {
                // Es el primero de la lista
                lista->cabeza = actual->siguiente;
            } else {
                // Está en medio o al final
                anterior->siguiente = actual->siguiente;
            }
            
            // Liberar SOLO el NodoLista (el contenedor), NO el Nodo (los datos)
            delete actual;
            
          
            
            // Retornar el Nodo intacto con todo su contenido
            return nodoRescatado;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    
    cout << "[DEBUG] No se encontró el nodo '" << nombreNodo << "'" << endl;
    return nullptr;
}

#endif