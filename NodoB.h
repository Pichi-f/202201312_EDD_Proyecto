#ifndef NODOB_H
#define NODOB_H

#include "nodoAviones.h"
#include <string>
#include <vector>

class NodoB {
public:
    std::vector<nodoAviones> clave;
    std::vector<NodoB*> puntero;
    int clavesUsadas;
    bool hoja;
    NodoB* siguiente;

    NodoB(int m) : clavesUsadas(0), hoja(true) {
        clave.resize(m - 1);
        puntero.resize(m);
        siguiente = nullptr;
    }

    bool esHoja() const {
        return hoja;
    }

    bool estaLleno() const {
        return clavesUsadas == clave.size();
    }

    void insertarClave(const nodoAviones& nuevoAvion) {
        int i = clavesUsadas - 1;
        while (i >= 0 && nuevoAvion.numero_de_registro < clave[i].numero_de_registro) {
            clave[i + 1] = clave[i];
            i--;
        }
        clave[i + 1] = nuevoAvion;
        clavesUsadas++;
    }

    int encontrarPosicionInsertar(const std::string& numero_registro) {
        int pos = 0;
        while (pos < clavesUsadas && numero_registro > clave[pos].numero_de_registro) {
            pos++;
        }
        return pos;
    }

    NodoB* dividirNodo(nodoAviones& mediana) {
        int mid = clavesUsadas / 2;
        NodoB* nuevoNodo = new NodoB(clave.size() + 1);
        nuevoNodo->hoja = hoja;
        nuevoNodo->clavesUsadas = clavesUsadas - mid - 1;

        for (int i = 0; i < nuevoNodo->clavesUsadas; ++i) {
            nuevoNodo->clave[i] = clave[mid + 1 + i];
        }

        if (!hoja) {
            for (int i = 0; i <= nuevoNodo->clavesUsadas; ++i) {
                nuevoNodo->puntero[i] = puntero[mid + 1 + i];
            }
        }

        clavesUsadas = mid;
        mediana = clave[mid];

        return nuevoNodo;
    }

    void eliminarClave(const std::string& numeroDeRegistro) {
        // Implementación específica de eliminación según las reglas del árbol B
    }

    ~NodoB() {
        for (auto& ptr : puntero) {
            delete ptr;
        }
    }
};

#endif // NODOB_H