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
        void insertarNodoLleno(NodoB* nodo, const nodoAviones& nuevoAvion);
        void destruirNodo(NodoB* nodo);
        void graficarNodo(stringstream& ss, NodoB* nodo, int& nullCount);
        void imprimirClavesOrdenadas(NodoB* nodo);
        void insertar(const nodoAviones& nuevoAvion);
        void eliminar(const string& numero_de_registro);
        void graficar(const string& nombreArchivo);
        void imprimirClavesOrdenadas();
        void ejecutarComandos(const string& comando);
        string obtenerDot();
        virtual ~ArbolB();
};

ArbolB::ArbolB(int m) : raiz(new NodoB(m)), m(m) {}

ArbolB::~ArbolB() {
    destruirNodo(raiz);
}

void ArbolB::destruirNodo(NodoB* nodo) {
    if (nodo) {
        if (!nodo->esHoja()) {
            for (int i = 0; i <= nodo->clavesUsadas; ++i) {
                destruirNodo(nodo->puntero[i]);
            }
        }
        delete nodo;
    }
}

void ArbolB::insertar(const nodoAviones& nuevoAvion) {
    if (raiz->estaLleno()) {
        NodoB* nuevaRaiz = new NodoB(m);
        nuevaRaiz->hoja = false;
        nuevaRaiz->puntero[0] = raiz;
        nodoAviones mediana;
        NodoB* nuevoNodo = raiz->dividirNodo(mediana);
        nuevaRaiz->clave[0] = mediana;
        nuevaRaiz->puntero[1] = nuevoNodo;
        nuevaRaiz->clavesUsadas = 1;
        raiz = nuevaRaiz;
    }
    insertarNodoLleno(raiz, nuevoAvion);
}

void ArbolB::insertarNodoLleno(NodoB* nodo, const nodoAviones& nuevoAvion) {
    if (nodo->esHoja()) {
        nodo->insertarClave(nuevoAvion);
    } else {
        int i = nodo->encontrarPosicionInsertar(nuevoAvion.numero_de_registro);
        if (nodo->puntero[i]->estaLleno()) {
            nodoAviones mediana;
            NodoB* nuevoNodo = nodo->puntero[i]->dividirNodo(mediana);
            nodo->insertarClave(mediana);
            if (nuevoAvion.numero_de_registro > mediana.numero_de_registro) {
                i++;
            }
        }
        insertarNodoLleno(nodo->puntero[i], nuevoAvion);
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
            raiz = raiz->puntero[0];
        }
        delete temp;
    }
}

void ArbolB::graficar(const string& nombreArchivo) {
    ofstream archivo(nombreArchivo + ".dot");
    archivo << obtenerDot();
    archivo.close();

    string comandoDot = "dot -Tpng " + nombreArchivo + ".dot -o " + nombreArchivo + ".png";
    ejecutarComandos(comandoDot);

    #ifdef _WIN32
        string comandoAbrir = "start " + nombreArchivo + ".png";
    #elif __APPLE__
        string comandoAbrir = "open " + nombreArchivo + ".png";
    #else
        string comandoAbrir = "xdg-open " + nombreArchivo + ".png";
    #endif
    ejecutarComandos(comandoAbrir);
}

string ArbolB::obtenerDot() {
    stringstream ss;
    ss << "digraph BTree {\n";
    ss << "splines=false;\n";
    ss << "node [shape=rect];\n";
    if (raiz) {
        int nullCount = 0;
        graficarNodo(ss, raiz, nullCount);
    }
    ss << "}\n";
    return ss.str();
}

void ArbolB::graficarNodo(stringstream& ss, NodoB* nodo, int& nullCount) {
    if (!nodo) return;

    ss << "node" << nodo << " [label=<\n";
    ss << "<table border=\"0\" cellspacing=\"0\" cellborder=\"1\">\n";
    ss << "<tr>\n";
    for (int i = 0; i < nodo->clavesUsadas; ++i) {
        ss << "<td border=\"0\"></td>\n";
        ss << "<td border=\"0\">" << nodo->clave[i].numero_de_registro << "</td>\n";
    }
    ss << "<td border=\"0\"></td>\n";
    ss << "</tr>\n";
    ss << "<tr>\n";
    for (int i = 0; i <= nodo->clavesUsadas; ++i) {
        ss << "<td port=\"p" << i << "\" border=\"0\" width=\"5\" height=\"5\" fixedsize=\"true\" style=\"rounded\"></td>\n";
        if (i < nodo->clavesUsadas) {
            ss << "<td border=\"0\"></td>\n";
        }
    }
    ss << "</tr>\n";
    ss << "</table>>];\n";
    if (!nodo->esHoja()) {
        for (int i = 0; i <= nodo->clavesUsadas; ++i) {
            if (nodo->puntero[i]) {
                ss << "node" << nodo << ":p" << i << " -> node" << nodo->puntero[i] << ";\n";
                graficarNodo(ss, nodo->puntero[i], nullCount);
            } else {
                ss << "null" << nullCount << " [shape=point];\n";
                ss << "node" << nodo << ":p" << i << " -> null" << nullCount << ";\n";
                nullCount++;
            }
        }
    }
    if (nodo->esHoja() && nodo->siguiente) {
        ss << "node" << nodo << " -> node" << nodo->siguiente << " [style=dashed, color=gray]\n";
    }
}

void ArbolB::ejecutarComandos(const string& comando) {
    system(comando.c_str());
}

void ArbolB::imprimirClavesOrdenadas() {
    if (raiz) {
        imprimirClavesOrdenadas(raiz);
    }
    cout << endl;
}

void ArbolB::imprimirClavesOrdenadas(NodoB* nodo) {
    if (!nodo) return;

    int i;
    for (i = 0; i < nodo->clavesUsadas; ++i) {
        if (!nodo->esHoja()) {
            imprimirClavesOrdenadas(nodo->puntero[i]);
        }
        cout << nodo->clave[i].numero_de_registro << " ";
    }
    if (!nodo->esHoja()) {
        imprimirClavesOrdenadas(nodo->puntero[i]);
    }
}

#endif