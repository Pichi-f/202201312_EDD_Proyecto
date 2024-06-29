#ifndef ARBOLB_H
#define ARBOLB_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "NodoB.h"

using namespace std;

class ArbolB {
    public:
        NodoB* raiz;
        int m;
        ArbolB(int m);
        void destroyNodo(NodoB* nodo);
        void insertar(const nodoAviones& nuevoAvion);
        void insertarNodo(NodoB* nodo, const nodoAviones& nuevoAvion);
        void eliminar(const string& numero_de_registro);
        void graficarNodos(stringstream& var, NodoB* nodo, int& contadorNulo);
        void imprimirClaves(NodoB* nodo);
        void graficar(const string& nombreArchivo);
        void imprimirClaves();
        void ejecutar(const string& comando);
        string generarDot();
        virtual ~ArbolB();
};

ArbolB::ArbolB(int m) : raiz(new NodoB(m)), m(m) {}

ArbolB::~ArbolB() {
    destroyNodo(raiz);
}

void ArbolB::destroyNodo(NodoB* nodo) {
    if (nodo) {
        if (!nodo->esHoja()) {
            for (int i = 0; i <= nodo->clavesUsadas; ++i) {
                destroyNodo(nodo->punteros[i]);
            }
        }
        delete nodo;
    }
}

void ArbolB::insertar(const nodoAviones& nuevoAvion) {
    if (raiz->lleno()) {
        NodoB* nueva = new NodoB(m);
        nueva->hoja = false;
        nueva->punteros[0] = raiz;
        nodoAviones mediana;
        NodoB* nuevoNodo = raiz->dividirNodo(mediana);
        nueva->claves[0] = mediana;
        nueva->punteros[1] = nuevoNodo;
        nueva->clavesUsadas = 1;
        raiz = nueva;
    }
    insertarNodo(raiz, nuevoAvion);
}

void ArbolB::insertarNodo(NodoB* nodo, const nodoAviones& nuevoAvion) {
    if (nodo->esHoja()) {
        nodo->insertarClave(nuevoAvion);
    } else {
        int i = nodo->posicionInsertar(nuevoAvion.numero_de_registro);
        if (nodo->punteros[i]->lleno()) {
            nodoAviones mediana;
            NodoB* nuevoNodo = nodo->punteros[i]->dividirNodo(mediana);
            nodo->insertarClave(mediana);
            nodo->punteros[i + 1] = nuevoNodo;
            if (nuevoAvion.numero_de_registro > mediana.numero_de_registro) {
                i++;
            }
        }
        insertarNodo(nodo->punteros[i], nuevoAvion);
    }
}

void ArbolB::eliminar(const string& numero_de_registro) {
    if (!raiz) {
        cout << "El arbol está vacío" << endl;
        return;
    }
    raiz->eliminarClave(numero_de_registro);
    if (raiz->clavesUsadas == 0) {
        NodoB* temp = raiz;
        if (raiz->esHoja()) {
            raiz = nullptr;
        } else {
            raiz = raiz->punteros[0];
        }
        delete temp;
    }
}


string ArbolB::generarDot() {
    stringstream var;
    var << "digraph ArbolB {\n";
    var << "node [shape=rect];\n";
    if (raiz) {
        int contadorNulo = 0;
        graficarNodos(var, raiz, contadorNulo);
    }
    var << "}\n";
    return var.str();
}

void ArbolB::graficarNodos(stringstream& var, NodoB* nodo, int& contadorNulo) {
    if (!nodo) return;

    var << "node" << nodo << " [label=<\n";
    var << "<table border=\"0\" cellspacing=\"0\" cellborder=\"1\">\n";

    var << "<tr>\n" << "<td border=\"0\"></td>\n"; 
    for (int i = 0; i < nodo->clavesUsadas; ++i) {
        if (i > 0) {
            var << "<td border=\"0\">| </td>\n"; // Agrega un separador antes de cada número, excepto el primero
        }
        var << "<td border=\"0\">" << nodo->claves[i].numero_de_registro << "</td>\n";
    }
    var << "<td border=\"0\"></td>\n</tr>\n"; 

    var << "<tr>\n";
    for (int i = 0; i <= nodo->clavesUsadas; ++i) {
        var << "<td port=\"p" << i << "\" border=\"0\" width=\"5\" height=\"5\" fixedsize=\"true\"></td>\n";
        if (i < nodo->clavesUsadas) {
            var << "<td border=\"0\"></td>\n";
        }
    }
    var << "</tr>\n</table>>];\n";

    if (!nodo->esHoja()) {
        for (int i = 0; i <= nodo->clavesUsadas; ++i) {
            if (nodo->punteros[i]) {
                var << "node" << nodo << ":p" << i << " -> node" << nodo->punteros[i] << ";\n";
                graficarNodos(var, nodo->punteros[i], contadorNulo);
            } else {
                var << "null" << contadorNulo << " [shape=point];\n";
                var << "node" << nodo << ":p" << i << " -> null" << contadorNulo++ << ";\n";
            }
        }
    } else if (nodo->siguiente) {
        var << "node" << nodo << " -> node" << nodo->siguiente << " [style=dashed, color=gray];\n";
    }
}

void ArbolB::ejecutar(const string& comando) {
    system(comando.c_str());
}

void ArbolB::imprimirClaves() {
    if (raiz) {
        imprimirClaves(raiz);
    }
    cout << endl;
}

void ArbolB::imprimirClaves(NodoB* nodo) {
    if (!nodo){ 
        return;
    }
    int temp;
    for (temp = 0; temp < nodo->clavesUsadas; ++temp) {
        if (!nodo->esHoja()) {
            imprimirClaves(nodo->punteros[temp]);
        }
        cout << nodo->claves[temp].numero_de_registro << " ";
    }
    if (!nodo->esHoja()) {
        imprimirClaves(nodo->punteros[temp]);
    }
}

void ArbolB::graficar(const string& nombreArchivo) {
    ofstream archivo(nombreArchivo + ".dot");
    archivo << generarDot();
    archivo.close();

    string comandoDot = "dot -Tpng " + nombreArchivo + ".dot -o " + nombreArchivo + ".png";
    ejecutar(comandoDot);

    #ifdef _WIN32
        string comandoAbrir = "start " + nombreArchivo + ".png";
    #elif __APPLE__
        string comandoAbrir = "open " + nombreArchivo + ".png";
    #else
        string comandoAbrir = "xdg-open " + nombreArchivo + ".png";
    #endif
    ejecutar(comandoAbrir);
}

#endif