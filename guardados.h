#ifndef GUARDADOS_H
#define GUARDADOS_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring> 
#include <cerrno>  
#include "estructuras.h"

using namespace std;

// PROTOTIPOS
void guardarSistema(Nodo *raiz, string nombreArchivo);
Nodo* cargarSistema(string nombreArchivo);
void guardarNodoRecursivo(Nodo* nodo, int nivel, ofstream& archivo);
Nodo *procesarLinea(string linea, int &nivel, TipoNodo &tipo, string &nombre, string &contenido);

// IMPLEMENTACION

inline void guardarSistema(Nodo* raiz, string nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        guardarNodoRecursivo(raiz, 0, archivo);
        archivo.close();
        cout << "[OK] Sistema guardado correctamente." << endl;
    } else {
        cout << "[ERROR] No se pudo guardar el sistema." << endl;
    }
}

inline void guardarNodoRecursivo(Nodo *nodo, int nivel, ofstream &archivo){
    char tipoChar = (nodo->tipo == CARPETA) ? 'D' : 'F';
    archivo << nivel << " " << tipoChar << " " << nodo->nombre;

    if (nodo->tipo == ARCHIVO && !nodo->contenido.empty()) {
        string contenidoSafe = nodo->contenido;
        
        for (size_t i = 0; i < contenidoSafe.length(); ++i) {
            if (contenidoSafe[i] == '\n') {
                contenidoSafe[i] = '#';
            } else if (contenidoSafe[i] == '\r') {
                contenidoSafe[i] = '@';
            }
        }
        
        archivo << " |TXT|" << contenidoSafe;
    }

    archivo << endl;

    if(nodo->tipo == CARPETA && nodo->hijos != nullptr){
        NodoLista *aux = nodo->hijos->cabeza;
        while(aux != nullptr){
            guardarNodoRecursivo(aux->datos, nivel + 1, archivo);
            aux = aux->siguiente;
        }
    }
}

inline Nodo *cargarSistema(string nombreArchivo){
    ifstream archivo(nombreArchivo);
    if(!archivo.is_open()){
        cout << "[SISTEMA] Archivo no encontrado. Iniciando sistema nuevo..." << endl;
        return crearNodo("/", CARPETA, nullptr); 
    }

    Nodo *padres[50];
    string linea;
    Nodo *raiz = nullptr;

    while (getline(archivo, linea))
    {
        if (linea.empty()) continue; 
        
        int nivel;
        TipoNodo tipo;
        string nombre;
        string contenido;
        
        Nodo *nuevoNodo = procesarLinea(linea, nivel, tipo, nombre, contenido);

        if(nuevoNodo != nullptr){
            nuevoNodo->contenido = contenido;
            
            if (nivel == 0) {
                raiz = nuevoNodo;
                padres[0] = raiz;
            } else {
                Nodo *padre = padres[nivel - 1];
                nuevoNodo->padre = padre;
                insertarEnLista(padre->hijos, nuevoNodo);
                padres[nivel] = nuevoNodo;
            }
        }
    }
    archivo.close();
    cout << "[OK] Sistema cargado correctamente." << endl;
    return raiz;
}

inline Nodo *procesarLinea(string linea, int &nivel, TipoNodo &tipo, string &nombre, string &contenido){
    size_t pos1 = linea.find(' ');
    size_t pos2 = linea.find(' ', pos1 + 1);

    if(pos1 == string::npos || pos2 == string::npos) return nullptr;

    nivel = stoi(linea.substr(0, pos1));
    tipo = (linea[pos1 + 1] == 'D') ? CARPETA : ARCHIVO;

    size_t posContenido = linea.find(" |TXT|");
    contenido = "";

    if (posContenido != string::npos) {
        nombre = linea.substr(pos2 + 1, posContenido - (pos2 + 1));
        contenido = linea.substr(posContenido + 6);
        
        for (size_t i = 0; i < contenido.length(); ++i) {
            if (contenido[i] == '#') {
                contenido[i] = '\n';
            } else if (contenido[i] == '@') {
                contenido[i] = '\r';
            }
        }
    } else {
        nombre = linea.substr(pos2 + 1);
    }

    Nodo* nodo = crearNodo(nombre, tipo, nullptr);
    return nodo;
}

#endif