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
    ~ArbolB();

    void destroyNodo(NodoB* nodo);
    void insertar(const nodoAviones& nuevoAvion);
    void insertarNodo(NodoB* nodo, const nodoAviones& nuevoAvion);
    void eliminar(const string& numero_de_registro);
    void eliminarRecursivo(NodoB* nodo, const string& numero_de_registro);
    void eliminarClaveEnHoja(NodoB* nodo, int idx);
    void eliminarClaveEnNodoInterno(NodoB* nodo, int idx);
    void llenar(NodoB* nodo, int idx);
    NodoB* obtenerPredecesor(NodoB* nodo);
    NodoB* obtenerSucesor(NodoB* nodo);
    void prestarDeAnterior(NodoB* nodo, int idx);
    void prestarDeSiguiente(NodoB* nodo, int idx);
    void fusionar(NodoB* nodo, int idx);
    void graficarNodos(stringstream& var, NodoB* nodo, int& contadorNulo);
    void imprimirClaves(NodoB* nodo);
    void graficar(const string& nombreArchivo);
    void imprimirClaves();
    void ejecutar(const string& comando);
    string generarDot();
    nodoAviones* buscar(const string& numero_de_registro);

private:
    int minimo() { return (m + 1) / 2 - 1; } 
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
    eliminarRecursivo(raiz, numero_de_registro);
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

void ArbolB::eliminarRecursivo(NodoB* nodo, const string& numero_de_registro) {
    int idx = 0;
    while (idx < nodo->clavesUsadas && numero_de_registro > nodo->claves[idx].numero_de_registro) {
        idx++;
    }

    if (idx < nodo->clavesUsadas && nodo->claves[idx].numero_de_registro == numero_de_registro) {
        if (nodo->esHoja()) {
            eliminarClaveEnHoja(nodo, idx);
        } else {
            eliminarClaveEnNodoInterno(nodo, idx);
        }
    } else {
        if (nodo->esHoja()) {
            cout << "La clave " << numero_de_registro << " no existe en el árbol." << endl;
            return;
        }

        bool flag = ((idx == nodo->clavesUsadas) ? true : false);
        if (nodo->punteros[idx]->clavesUsadas < minimo()) {
            llenar(nodo, idx);
        }

        if (flag && idx > nodo->clavesUsadas) {
            eliminarRecursivo(nodo->punteros[idx - 1], numero_de_registro);
        } else {
            eliminarRecursivo(nodo->punteros[idx], numero_de_registro);
        }
    }
}

void ArbolB::eliminarClaveEnHoja(NodoB* nodo, int idx) {
    for (int i = idx + 1; i < nodo->clavesUsadas; ++i) {
        nodo->claves[i - 1] = nodo->claves[i];
    }
    nodo->clavesUsadas--; 
}

void ArbolB::eliminarClaveEnNodoInterno(NodoB* nodo, int idx) {
    string clave = nodo->claves[idx].numero_de_registro;
    if (nodo->punteros[idx]->clavesUsadas >= minimo()) {
        NodoB* temp = obtenerPredecesor(nodo->punteros[idx]);
        nodo->claves[idx] = temp->claves[temp->clavesUsadas - 1];
        eliminarRecursivo(nodo->punteros[idx], temp->claves[temp->clavesUsadas - 1].numero_de_registro);
    } else if (nodo->punteros[idx + 1]->clavesUsadas >= minimo()) {
        NodoB* temp = obtenerSucesor(nodo->punteros[idx + 1]);
        nodo->claves[idx] = temp->claves[0];
        eliminarRecursivo(nodo->punteros[idx + 1], temp->claves[0].numero_de_registro);
    } else {
        fusionar(nodo, idx);
        eliminarRecursivo(nodo->punteros[idx], clave);
    }
}

void ArbolB::llenar(NodoB* nodo, int idx) {
    if (idx != 0 && nodo->punteros[idx - 1]->clavesUsadas >= minimo()) {
        prestarDeAnterior(nodo, idx);
    } else if (idx != nodo->clavesUsadas && nodo->punteros[idx + 1]->clavesUsadas >= minimo()) {
        prestarDeSiguiente(nodo, idx);
    } else {
        if (idx != nodo->clavesUsadas) {
            fusionar(nodo, idx);
        } else {
            fusionar(nodo, idx - 1);
        }
    }
}

NodoB* ArbolB::obtenerPredecesor(NodoB* nodo) {
    while (!nodo->esHoja()) {
        nodo = nodo->punteros[nodo->clavesUsadas];
    }
    return nodo;
}

NodoB* ArbolB::obtenerSucesor(NodoB* nodo) {
    while (!nodo->esHoja()) {
        nodo = nodo->punteros[0];
    }
    return nodo;
}

void ArbolB::prestarDeAnterior(NodoB* nodo, int idx) {
    NodoB* hijo = nodo->punteros[idx];
    NodoB* hermano = nodo->punteros[idx - 1];

    for (int i = hijo->clavesUsadas - 1; i >= 0; --i) {
        hijo->claves[i + 1] = hijo->claves[i];
    }

    if (!hijo->esHoja()) {
        for (int i = hijo->clavesUsadas; i >= 0; --i) {
            hijo->punteros[i + 1] = hijo->punteros[i];
        }
    }

    hijo->claves[0] = nodo->claves[idx - 1];

    if (!hijo->esHoja()) {
        hijo->punteros[0] = hermano->punteros[hermano->clavesUsadas];
    }

    nodo->claves[idx - 1] = hermano->claves[hermano->clavesUsadas - 1];

    hijo->clavesUsadas++;
    hermano->clavesUsadas--;
}

void ArbolB::prestarDeSiguiente(NodoB* nodo, int idx) {
    NodoB* hijo = nodo->punteros[idx];
    NodoB* hermano = nodo->punteros[idx + 1];

    hijo->claves[hijo->clavesUsadas] = nodo->claves[idx];

    if (!hijo->esHoja()) {
        hijo->punteros[hijo->clavesUsadas + 1] = hermano->punteros[0];
    }

    nodo->claves[idx] = hermano->claves[0];

    for (int i = 1; i < hermano->clavesUsadas; ++i) {
        hermano->claves[i - 1] = hermano->claves[i];
    }

    if (!hermano->esHoja()) {
        for (int i = 1; i <= hermano->clavesUsadas; ++i) {
            hermano->punteros[i - 1] = hermano->punteros[i];
        }
    }

    hijo->clavesUsadas++;
    hermano->clavesUsadas--;
}

void ArbolB::fusionar(NodoB* nodo, int idx) {
    NodoB* hijo = nodo->punteros[idx];
    NodoB* hermano = nodo->punteros[idx + 1];

    hijo->claves[minimo()] = nodo->claves[idx];

    for (int i = 0; i < hermano->clavesUsadas; ++i) {
        hijo->claves[i + minimo() + 1] = hermano->claves[i];
    }

    if (!hijo->esHoja()) {
        for (int i = 0; i <= hermano->clavesUsadas; ++i) {
            hijo->punteros[i + minimo() + 1] = hermano->punteros[i];
        }
    }

    for (int i = idx + 1; i < nodo->clavesUsadas; ++i) {
        nodo->claves[i - 1] = nodo->claves[i];
    }

    for (int i = idx + 2; i <= nodo->clavesUsadas; ++i) {
        nodo->punteros[i - 1] = nodo->punteros[i];
    }

    hijo->clavesUsadas += hermano->clavesUsadas + 1;
    nodo->clavesUsadas--;

    delete hermano;
}

void ArbolB::graficarNodos(stringstream& var, NodoB* nodo, int& contadorNulo) {
    var << "node" << nodo << " [label=\"";
    for (int i = 0; i < nodo->clavesUsadas; ++i) {
        var << "<f" << i << "> |" << nodo->claves[i].numero_de_registro << "|";
    }
    var << "<f" << nodo->clavesUsadas << ">\"];\n";
    for (int i = 0; i <= nodo->clavesUsadas; ++i) {
        if (nodo->punteros[i]) {
            var << "node" << nodo << ":f" << i << " -> node" << nodo->punteros[i] << ";\n";
            graficarNodos(var, nodo->punteros[i], contadorNulo);
        } else {
            var << "node" << nodo << ":f" << i << " -> nodeNulo" << contadorNulo << ";\n";
            var << "nodeNulo" << contadorNulo << " [label=\"Nulo\", shape=point];\n";
            contadorNulo++;
        }
    }
}

void ArbolB::imprimirClaves(NodoB* nodo) {
    for (int i = 0; i < nodo->clavesUsadas; ++i) {
        if (!nodo->esHoja()) {
            imprimirClaves(nodo->punteros[i]);
        }
        cout << nodo->claves[i].numero_de_registro << " ";
    }
    if (!nodo->esHoja()) {
        imprimirClaves(nodo->punteros[nodo->clavesUsadas]);
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

void ArbolB::imprimirClaves() {
    if (raiz != nullptr) {
        imprimirClaves(raiz);
        cout << endl;
    } else {
        cout << "El árbol está vacío." << endl;
    }
}

void ArbolB::ejecutar(const string& comando) {
    system(comando.c_str());
}

string ArbolB::generarDot() {
    stringstream var;
    int contadorNulo = 0;
    var << "digraph G {\n";
    var << "node [shape = record,height=.1];\n";
    graficarNodos(var, raiz, contadorNulo);
    var << "}\n";
    return var.str();
}

nodoAviones* ArbolB::buscar(const string& numero_de_registro) {
    NodoB* actual = raiz;
    while (actual != nullptr) {
        int i = 0;
        while (i < actual->clavesUsadas && numero_de_registro > actual->claves[i].numero_de_registro) {
            i++;
        }
        if (i < actual->clavesUsadas && numero_de_registro == actual->claves[i].numero_de_registro) {
            return &actual->claves[i];
        }
        actual = actual->punteros[i];
    }
    return nullptr;
}

#endif // ARBOLB_H