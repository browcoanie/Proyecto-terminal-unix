#include <iostream>
#include "estructuras.h" 

using namespace std;

int main() {
    cout << "--- PRUEBA DE ESTRUCTURAS (ETAPA 1) ---" << endl;

    // 1. Creamos la raíz "/"
    Nodo* raiz = crearNodo("/", CARPETA, nullptr);

    // 2. Creamos una carpeta "home" dentro de raíz
    Nodo* home = crearNodo("home", CARPETA, raiz);
    insertarEnLista(raiz->hijos, home);

    // 3. Creamos una carpeta "usuario" dentro de home
    Nodo* user = crearNodo("usuario", CARPETA, home);
    insertarEnLista(home->hijos, user);

    // 4. Creamos un archivo "nota.txt" dentro de usuario
    Nodo* archivo = crearNodo("nota.txt", ARCHIVO, user);
    insertarEnLista(user->hijos, archivo);

    // 5. Imprimimos para ver si se guardó bien la jerarquía
    imprimirEstructura(raiz, 0);

    return 0;
}