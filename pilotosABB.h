#include <iostream>
#include <string>
#include <fstream>
#include "nodoPiloto.h"
#include <vector>   

using namespace std;

class pilotosABB {
public:
    ofstream archivo;
    nodoPiloto *raiz;
    int nodoDato;
    int recorrido = 0;
    pilotosABB();
    bool esVacio();
    void insertar(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia);
    nodoPiloto* insertarNodo(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia, nodoPiloto *nodoPtr);
    void generarReporte();
    void imprimirNodo(nodoPiloto* nodoPtr);
    void preOrden(nodoPiloto *);
    void inOrden(nodoPiloto *);
    void postOrden(nodoPiloto *);
    void eliminar(string numero_de_id);
    nodoPiloto* eliminarNodo(nodoPiloto* raiz, string numero_de_id);
    nodoPiloto* encontrarMinimo(nodoPiloto* nodo); 
    virtual ~pilotosABB();
};

pilotosABB::pilotosABB() {
    raiz = nullptr;
}

bool pilotosABB::esVacio() {
    return raiz == nullptr;
}

void pilotosABB::insertar(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia) {
    raiz = insertarNodo(nombre, nacionalidad, numero_de_id, vuelo, horas_de_vuelo, tipo_de_licencia, raiz);
}

nodoPiloto* pilotosABB::insertarNodo(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia, nodoPiloto *nodoPtr) {
    if (nodoPtr == nullptr) {
        nodoPtr = new nodoPiloto(nombre, nacionalidad, numero_de_id, vuelo, horas_de_vuelo, tipo_de_licencia);
    } else if (horas_de_vuelo < nodoPtr->horas_de_vuelo) {
        nodoPtr->izquierdo = insertarNodo(nombre, nacionalidad, numero_de_id, vuelo, horas_de_vuelo, tipo_de_licencia, nodoPtr->izquierdo);
    } else if (horas_de_vuelo > nodoPtr->horas_de_vuelo) {
        nodoPtr->derecho = insertarNodo(nombre, nacionalidad, numero_de_id, vuelo, horas_de_vuelo, tipo_de_licencia, nodoPtr->derecho);
    } else {
        cout << "El piloto ya existe" << endl;
    }
    return nodoPtr;
}

pilotosABB::~pilotosABB() {
}

void pilotosABB::preOrden(nodoPiloto *nodoPtr) {
    if (nodoPtr == nullptr) {
        return;
    }
    cout << nodoPtr->horas_de_vuelo << " - ";
    preOrden(nodoPtr->izquierdo);
    preOrden(nodoPtr->derecho);
}

void pilotosABB::inOrden(nodoPiloto *nodoPtr) {
    if (nodoPtr == nullptr) {
        return;
    }
    inOrden(nodoPtr->izquierdo);
    cout << nodoPtr->horas_de_vuelo << " - ";
    inOrden(nodoPtr->derecho);
}

void pilotosABB::postOrden(nodoPiloto *nodoPtr) {
    if (nodoPtr == nullptr) {
        return;
    }
    postOrden(nodoPtr->izquierdo);
    postOrden(nodoPtr->derecho);
    cout << nodoPtr->horas_de_vuelo << " - ";
}

void pilotosABB::generarReporte() {
    if (!esVacio()) {
        archivo.open("pilotos.dot", ios::out);
        archivo << "digraph G {" << endl;

        imprimirNodo(raiz);

        archivo << "}" << endl;
        archivo.close();
        system("dot -Tpng pilotos.dot -o pilotos.png");
        system("pilotos.png");
    }
}

void pilotosABB::imprimirNodo(nodoPiloto* nodoPtr) {
    if (nodoPtr == nullptr) {
        return;
    }
    if (nodoPtr->izquierdo != nullptr) {
        nodoDato = nodoPtr->horas_de_vuelo;
        archivo << nodoDato;
        archivo << "->";
        nodoDato = nodoPtr->izquierdo->horas_de_vuelo;
        archivo << nodoDato;
        archivo << ";";
    }
    imprimirNodo(nodoPtr->izquierdo);

    if (nodoPtr->derecho != nullptr) {
        nodoDato = nodoPtr->horas_de_vuelo;
        archivo << nodoDato;
        archivo << "->";
        nodoDato = nodoPtr->derecho->horas_de_vuelo;
        archivo << nodoDato;
        archivo << ";";
    }
    imprimirNodo(nodoPtr->derecho);
}

void pilotosABB::eliminar(string numero_de_id) {
    raiz = eliminarNodo(raiz, numero_de_id);
}

nodoPiloto* pilotosABB::eliminarNodo(nodoPiloto* raiz, string numero_de_id) {
    if (raiz == nullptr) {
        return nullptr;
    }

    // Si el número de ID a eliminar es menor que el del nodo actual, buscar en el subárbol izquierdo
    if (numero_de_id < raiz->numero_de_id) {
        raiz->izquierdo = eliminarNodo(raiz->izquierdo, numero_de_id);
    }
    // Si el número de ID a eliminar es mayor que el del nodo actual, buscar en el subárbol derecho
    else if (numero_de_id > raiz->numero_de_id) {
        raiz->derecho = eliminarNodo(raiz->derecho, numero_de_id);
    }
    // Si el número de ID a eliminar es igual al del nodo actual, proceder a eliminarlo
    else if (numero_de_id == raiz->numero_de_id) {
        // Caso 1: Nodo sin hijos
        if (raiz->izquierdo == nullptr && raiz->derecho == nullptr) {
            delete raiz;
            return nullptr;
        }
        // Caso 2: Nodo con un solo hijo
        else if (raiz->izquierdo == nullptr) {
            nodoPiloto* temp = raiz->derecho;
            delete raiz;
            return temp;
        }
        else if (raiz->derecho == nullptr) {
            nodoPiloto* temp = raiz->izquierdo;
            delete raiz;
            return temp;
        }
        // Caso 3: Nodo con dos hijos
        else {
            nodoPiloto* temp = encontrarMinimo(raiz->derecho);
            raiz->numero_de_id = temp->numero_de_id;
            raiz->horas_de_vuelo = temp->horas_de_vuelo;
            raiz->derecho = eliminarNodo(raiz->derecho, temp->numero_de_id);
        }
    }
    return raiz;
}

nodoPiloto* pilotosABB::encontrarMinimo(nodoPiloto* nodo) {
    while (nodo->izquierdo != nullptr) {
        nodo = nodo->izquierdo;
    }
    return nodo;
}