#include <iostream>
#include <string>
#include <sstream> 
#include "estructuras.h"
#include "guardados.h"
#include "comandos.h"

using namespace std;

void mostrarBanner() {
    cout << "===============================================" << endl;
    cout << "                                               " << endl;
    cout << "              TERMINAL UNIX                    " << endl;
    cout << "                                               " << endl;
    cout << "===============================================" << endl;
    cout << endl;
}

void mostrarAyuda() {
    cout << "[COMANDOS DISPONIBLES]" << endl;
    cout << "  ls              - Listar contenido del directorio" << endl;
    cout << "  mkdir <dir>     - Crear directorio" << endl;
    cout << "  cd <dir>        - Cambiar directorio" << endl;
    cout << "  cd ..           - Subir un nivel" << endl;
    cout << "  touch <file>    - Crear archivo" << endl;
    cout << "  edit <file>     - Editar archivo (modo append)" << endl;
    cout << "  cat <file>      - Mostrar contenido de archivo" << endl;
    cout << "  mv <src> <dst>  - Mover/renombrar" << endl;
    cout << "  rm <nombre>     - Eliminar archivo/directorio vacio" << endl;
    cout << "  tree            - Mostrar arbol de directorios" << endl;
    cout << "  help            - Mostrar esta ayuda" << endl;
    cout << "  exit            - Guardar y salir" << endl;
    cout << endl;
}

int main() {
    string archivoBD = "sistema_archivos.txt"; 

    mostrarBanner();
    
    Nodo* raiz = cargarSistema(archivoBD);
    Nodo* actual = raiz; 

    mostrarAyuda();

    string inputUsuario;
    bool salir = false;

    while (!salir) {
        mostrarPrompt(actual);
        
        getline(cin, inputUsuario);
        if (inputUsuario.empty()) continue;

        stringstream ss(inputUsuario);
        string comando, argumento, destino;
        
        ss >> comando;      
        ss >> argumento;    
        
        if (comando == "ls") {
            ejecutarLs(actual);
        }
        else if (comando == "mkdir") {
            if (argumento.empty()) {
                cout << "[USO] mkdir <nombre>" << endl;
            } else {
                ejecutarMkdir(actual, argumento);
            }
        }
        else if (comando == "touch") {
            if (argumento.empty()) {
                cout << "[USO] touch <nombre>" << endl;
            } else {
                ejecutarTouch(actual, argumento);
            }
        }
        else if (comando == "cd") {
            if (argumento.empty()) {
                cout << "[USO] cd <directorio>" << endl;
            } else {
                ejecutarCd(actual, argumento);
            }
        }
        else if (comando == "mv") {
            ss >> destino; 
            
            if (argumento.empty() || destino.empty()) {
                cout << "[USO] mv <origen> <destino>" << endl;
            } else {
                ejecutarMv(actual, argumento, destino);
            }
        }
        else if (comando == "edit") {
            if (argumento.empty()) {
                cout << "[USO] edit <archivo>" << endl;
            } else {
                ejecutarEdit(actual, argumento);
            }
        }
        else if (comando == "cat") {
            if (argumento.empty()) {
                cout << "[USO] cat <archivo>" << endl;
            } else {
                ejecutarCat(actual, argumento);
            }
        }
        else if (comando == "rm") {
            if (argumento.empty()) {
                cout << "[USO] rm <nombre>" << endl;
            } else {
                ejecutarRm(actual, argumento);
            }
        }
        else if (comando == "tree") {
            cout << endl;
            imprimirEstructura(raiz, 0);
            cout << endl;
        }
        else if (comando == "help" || comando == "?") {
            mostrarAyuda();
        }
        else if (comando == "clear" || comando == "cls") {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
            mostrarBanner();
        }
        else if (comando == "exit" || comando == "quit") {
            cout << "[SISTEMA] Guardando sistema..." << endl;
            guardarSistema(raiz, archivoBD);
            cout << "Hasta luego!" << endl;
            salir = true;
        }
        else {
            cout << "[ERROR] Comando '" << comando << "' no reconocido." << endl;
            cout << "Escribe 'help' para ver los comandos disponibles." << endl;
        }
    }

    return 0;
}