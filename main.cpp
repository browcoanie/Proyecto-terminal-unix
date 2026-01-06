#include <iostream>
#include <string>
#include <sstream> 
#include "estructuras.h"
#include "guardar.h"
#include "comandos.h"

using namespace std;

void mostrarBanner() {
    cout << "\033[1;36m===============================================\033[0m" << endl;
    cout << "\033[1;36m                                               \033[0m" << endl;
    cout << "\033[1;36m              \033[1;35mTERMINAL UNIX\033[0m             \033[0m" << endl;
    cout << "\033[1;36m                                               \033[0m" << endl;
    cout << "\033[1;36m===============================================\033[0m" << endl;
    cout << endl;
}

void mostrarAyuda() {
    cout << "\033[1;33m[COMANDOS DISPONIBLES]\033[0m" << endl;
    cout << "  \033[1;32mls\033[0m              - Listar contenido del directorio" << endl;
    cout << "  \033[1;32mmkdir\033[0m <dir>     - Crear directorio" << endl;
    cout << "  \033[1;32mcd\033[0m <dir>        - Cambiar directorio" << endl;
    cout << "  \033[1;32mcd ..\033[0m           - Subir un nivel" << endl;
    cout << "  \033[1;32mtouch\033[0m <file>    - Crear archivo" << endl;
    cout << "  \033[1;32medit\033[0m <file>     - Editar archivo (modo append)" << endl;
    cout << "  \033[1;32mcat\033[0m <file>      - Mostrar contenido de archivo" << endl;
    cout << "  \033[1;32mmv\033[0m <src> <dst>  - Mover/renombrar" << endl;
    cout << "  \033[1;32mrm\033[0m <nombre>     - Eliminar archivo/directorio vacio" << endl;
    cout << "  \033[1;32mtree\033[0m            - Mostrar arbol de directorios" << endl;
    cout << "  \033[1;32mhelp\033[0m            - Mostrar esta ayuda" << endl;
    cout << "  \033[1;32mexit\033[0m            - Guardar y salir" << endl;
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
                cout << "\033[1;33m[USO]\033[0m mkdir <nombre>" << endl;
            } else {
                ejecutarMkdir(actual, argumento);
            }
        }
        else if (comando == "touch") {
            if (argumento.empty()) {
                cout << "\033[1;33m[USO]\033[0m touch <nombre>" << endl;
            } else {
                ejecutarTouch(actual, argumento);
            }
        }
        else if (comando == "cd") {
            if (argumento.empty()) {
                cout << "\033[1;33m[USO]\033[0m cd <directorio>" << endl;
            } else {
                ejecutarCd(actual, argumento);
            }
        }
        else if (comando == "mv") {
            ss >> destino; 
            
            if (argumento.empty() || destino.empty()) {
                cout << "\033[1;33m[USO]\033[0m mv <origen> <destino>" << endl;
            } else {
                ejecutarMv(actual, argumento, destino);
            }
        }
        else if (comando == "edit") {
            if (argumento.empty()) {
                cout << "\033[1;33m[USO]\033[0m edit <archivo>" << endl;
            } else {
                ejecutarEdit(actual, argumento);
            }
        }
        else if (comando == "cat") {
            if (argumento.empty()) {
                cout << "\033[1;33m[USO]\033[0m cat <archivo>" << endl;
            } else {
                ejecutarCat(actual, argumento);
            }
        }
        else if (comando == "rm") {
            if (argumento.empty()) {
                cout << "\033[1;33m[USO]\033[0m rm <nombre>" << endl;
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
            cout << "\033[1;33m[SISTEMA]\033[0m Guardando sistema...\033[0m" << endl;
            guardarSistema(raiz, archivoBD);
            cout << "\033[1;36mHasta luego!\033[0m" << endl;
            salir = true;
        }
        else {
            cout << "\033[1;31m[ERROR]\033[0m Comando '\033[1m" << comando << "\033[0m' no reconocido." << endl;
            cout << "\033[2mEscribe 'help' para ver los comandos disponibles.\033[0m" << endl;
        }
    }

    return 0;
}