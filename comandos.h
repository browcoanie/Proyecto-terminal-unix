#ifndef COMANDOS_H
#define COMANDOS_H

#include <iostream>
#include <string>
#include <sstream>
#include "estructuras.h"

using namespace std;

// PROTOTIPOS
void ejecutarLs(Nodo* actual);
void ejecutarMkdir(Nodo* actual, string nombreCarpeta);
void ejecutarTouch(Nodo* actual, string nombreArchivo);
void ejecutarCd(Nodo* &actual, string destino);
void mostrarPrompt(Nodo* actual);
void ejecutarMv(Nodo* actual, string origen, string destino);
void ejecutarEdit(Nodo* actual, string nombreArchivo);
void ejecutarCat(Nodo* actual, string nombreArchivo);
void ejecutarRm(Nodo* actual, string nombre);

// IMPLEMENTACIÓN

inline void ejecutarLs(Nodo* actual) {
    if (actual->hijos == nullptr || actual->hijos->cabeza == nullptr) {
        return;
    }

    NodoLista* aux = actual->hijos->cabeza;
    while (aux != nullptr) {
        if(aux->datos->tipo == CARPETA){
            cout << "[DIR]  " << aux->datos->nombre << endl;
        }else{
            cout << "[FILE] " << aux->datos->nombre;
            if (!aux->datos->contenido.empty()) {
                cout << " (" << aux->datos->contenido.length() << " bytes)";
            }
            cout << endl;
        }
        aux = aux->siguiente;
    }
}

inline void ejecutarMkdir(Nodo* actual, string nombreCarpeta) {
    NodoLista *aux = actual->hijos->cabeza;
    while(aux != nullptr){
        if (aux->datos->nombre == nombreCarpeta){
            cout << "[ERROR] El directorio '" << nombreCarpeta << "' ya existe." << endl;
            return;
        }
        aux = aux->siguiente;      
    }
    Nodo *nuevaCarpeta = crearNodo(nombreCarpeta, CARPETA, actual);
    insertarEnLista(actual->hijos, nuevaCarpeta);
    cout << "[OK] Directorio '" << nombreCarpeta << "' creado." << endl;
}

inline void ejecutarTouch(Nodo* actual, string nombreArchivo) {
    NodoLista* aux = actual->hijos->cabeza;
    while(aux != nullptr) {
        if(aux->datos->nombre == nombreArchivo) {
            cout << "[ERROR] El archivo '" << nombreArchivo << "' ya existe." << endl;
            return;
        }
        aux = aux->siguiente;
    }
    Nodo* nuevoArchivo = crearNodo(nombreArchivo, ARCHIVO, actual);
    insertarEnLista(actual->hijos, nuevoArchivo);
    cout << "[OK] Archivo '" << nombreArchivo << "' creado." << endl;
}

inline void ejecutarCd(Nodo *&actual, string destino) {
    if (destino == "..") {
        if (actual->padre != nullptr) {
            actual = actual->padre;
        } else{
            cout << "[INFO] Ya estas en la raiz." << endl;
        }
        return;
    }

    NodoLista* aux = actual->hijos->cabeza;
    bool encontrado = false;

    while (aux != nullptr) {
        if (aux->datos->nombre == destino) {
            if (aux->datos->tipo == CARPETA) {
                actual = aux->datos;
                encontrado = true;
            } else {
                cout << "[ERROR] '" << destino << "' no es un directorio." << endl;
                return;
            }
            break;
        }
        aux = aux->siguiente;
    }

    if (!encontrado) {
        cout << "[ERROR] Directorio '" << destino << "' no encontrado." << endl;
    }
}

inline void mostrarPrompt(Nodo* actual) {
    string ruta = "";
    Nodo* temp = actual;
    while(temp != nullptr) {
        if (temp->padre == nullptr) { 
            ruta = temp->nombre + ruta;
        } else {
             string slash = (ruta == "") ? "" : "/";
             ruta = temp->nombre + slash + ruta;
        }
        temp = temp->padre;
    }
    if (ruta.length() > 1 && ruta[0] != '/') ruta = "/" + ruta;
    
    cout << ruta << " $ ";
}

inline void ejecutarMv(Nodo* actual, string origen, string destino) {
    Nodo* nodoMover = buscarEnLista(actual->hijos, origen);
    if (nodoMover == nullptr) {
        cout << "[ERROR] No existe '" << origen << "'." << endl;
        return;
    }

    if (destino == "..") {
        if (actual->padre == nullptr) {
            cout << "[ERROR] No se puede mover a la raiz desde aqui." << endl;
            return;
        }
        
        Nodo* nodoRescatado = desconectarNodo(actual->hijos, origen);
        if (nodoRescatado == nullptr) {
            cout << "[ERROR] No se pudo desconectar el nodo." << endl;
            return;
        }
        
        nodoRescatado->padre = actual->padre;
        insertarEnLista(actual->padre->hijos, nodoRescatado);
        
        cout << "[OK] Movido a directorio superior." << endl;
        return;
    }

    Nodo* nodoDestino = buscarEnLista(actual->hijos, destino);

    if (nodoDestino == nullptr) {
        nodoMover->nombre = destino;
        cout << "[OK] Renombrado a: '" << destino << "'." << endl;
    } 
    else if (nodoDestino->tipo == CARPETA) {
        Nodo* nodoRescatado = desconectarNodo(actual->hijos, origen);
        if (nodoRescatado == nullptr) {
            cout << "[ERROR] No se pudo desconectar el nodo." << endl;
            return;
        }
        
        nodoRescatado->padre = nodoDestino;
        insertarEnLista(nodoDestino->hijos, nodoRescatado);
        
        cout << "[OK] Movido a: '" << destino << "'." << endl;
    } 
    else {
        cout << "[ERROR] El destino ya existe y no es un directorio." << endl;
    }
}

inline void ejecutarEdit(Nodo* actual, string nombreArchivo) {
    Nodo* nodoArchivo = buscarEnLista(actual->hijos, nombreArchivo);
    
    if (nodoArchivo == nullptr) {
        cout << "[ERROR] Archivo no encontrado." << endl;
        return;
    }
    if (nodoArchivo->tipo == CARPETA) {
        cout << "[ERROR] No se puede editar un directorio." << endl;
        return;
    }

    cout << endl;
    cout << "--- EDITOR: " << nombreArchivo << " ---" << endl;
    cout << endl;
    
    if (!nodoArchivo->contenido.empty()) {
        cout << nodoArchivo->contenido;
    } else {
        cout << "(archivo vacio)" << endl;
    }
    
    cout << endl;
    cout << "Comandos: :wq (guardar) | :clear (borrar todo)" << endl;
    cout << string(50, '-') << endl;

    string linea;
    string contenidoNuevo = "";
    bool modoLimpio = false;
    
    while (true) {
        getline(cin, linea);
        
        if (linea == ":wq") {
            break;
        }
        else if (linea == ":clear") {
            contenidoNuevo = "";
            modoLimpio = true;
            cout << "[INFO] Contenido borrado. Escribiendo desde cero..." << endl;
        }
        else {
            contenidoNuevo += linea + "\n";
        }
    }

    if (!modoLimpio && !nodoArchivo->contenido.empty()) {
        nodoArchivo->contenido += contenidoNuevo;
    } else {
        nodoArchivo->contenido = contenidoNuevo;
    }
    
    cout << "[OK] Guardado (" << nodoArchivo->contenido.length() << " bytes)" << endl;
    cout << endl;
}

inline void ejecutarCat(Nodo* actual, string nombreArchivo) {
    Nodo* nodoArchivo = buscarEnLista(actual->hijos, nombreArchivo);
    
    if (nodoArchivo == nullptr) {
        cout << "[ERROR] Archivo no encontrado." << endl;
        return;
    }
    if (nodoArchivo->tipo == CARPETA) {
        cout << "[ERROR] No se puede mostrar el contenido de un directorio." << endl;
        return;
    }

    if (nodoArchivo->contenido.empty()) {
        cout << "(archivo vacio)" << endl;
    } else {
        cout << nodoArchivo->contenido;
    }
}

inline void ejecutarRm(Nodo* actual, string nombre) {
    Nodo* nodoEliminar = buscarEnLista(actual->hijos, nombre);
    
    if (nodoEliminar == nullptr) {
        cout << "[ERROR] '" << nombre << "' no existe." << endl;
        return;
    }
    
    if (nodoEliminar->tipo == CARPETA && nodoEliminar->hijos->cabeza != nullptr) {
        cout << "[ERROR] No se puede eliminar un directorio no vacio." << endl;
        cout << "[INFO] Elimina primero su contenido." << endl;
        return;
    }
    
    desconectarNodo(actual->hijos, nombre);
    cout << "[OK] '" << nombre << "' eliminado." << endl;
}

#endif