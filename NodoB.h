#ifndef NODOB_H
#define NODOB_H

#include "nodoAviones.h"
#include <string>
#include <vector>

class NodoB {
public:
    std::vector<nodoAviones> claves;
    std::vector<NodoB*> punteros;
    int clavesUsadas;
    bool hoja;
    NodoB* siguiente;

    NodoB(int m) : clavesUsadas(0), hoja(true) {
        claves.resize(m - 1);
        punteros.resize(m);
        siguiente = nullptr;
    }

    bool esHoja() const {
        return hoja;
    }

    bool lleno() const {
        return clavesUsadas == claves.size();
    }

    void insertarClave(const nodoAviones& nuevoAvion) {
        int i = clavesUsadas - 1;
        while (i >= 0 && nuevoAvion.numero_de_registro < claves[i].numero_de_registro) {
            claves[i + 1] = claves[i];
            i--;
        }
        claves[i + 1] = nuevoAvion;
        clavesUsadas++;
    }

    int posicionInsertar(const std::string& numero_registro) {
        int pos = 0;
        while (pos < clavesUsadas && numero_registro > claves[pos].numero_de_registro) {
            pos++;
        }
        return pos;
    }

    NodoB* dividirNodo(nodoAviones& mediana) {
        int mid = clavesUsadas / 2;
        NodoB* nuevoNodo = new NodoB(claves.size() + 1);
        nuevoNodo->hoja = hoja;
        nuevoNodo->clavesUsadas = clavesUsadas - mid - 1;

        for (int i = 0; i < nuevoNodo->clavesUsadas; ++i) {
            nuevoNodo->claves[i] = claves[mid + 1 + i];
        }

        if (!hoja) {
            for (int i = 0; i <= nuevoNodo->clavesUsadas; ++i) {
                nuevoNodo->punteros[i] = punteros[mid + 1 + i];
            }
        }

        clavesUsadas = mid;
        mediana = claves[mid];

        return nuevoNodo;
    }

    void eliminarClave(const std::string& numeroDeRegistro) {
        // Implementación específica de eliminación según las reglas del árbol B
    }

    ~NodoB() {
        for (auto& ptr : punteros) {
            delete ptr;
        }
    }
};

#endif // NODOB_H