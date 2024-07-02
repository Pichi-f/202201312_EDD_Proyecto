#include <iostream>
#include <limits>
#include <string>
#include <utility>

class Nodo {
public:
    std::string ciudad;
    int distancia;
    Nodo* siguiente;
    Nodo* anterior;

    Nodo(const std::string& c, int d) : ciudad(c), distancia(d), siguiente(nullptr), anterior(nullptr) {}
};

class monticulo {
private:
    Nodo* inicio;
    Nodo* fin;

    void intercambiarNodos(Nodo* a, Nodo* b);
    void hundirNodo(Nodo* nodo);
    void elevarNodo(Nodo* nodo);

public:
    monticulo() : inicio(nullptr), fin(nullptr) {}
    bool estaVacio() const { return inicio == nullptr; }
    void insertarCiudad(const std::string& ciudad, int distancia);
    std::pair<std::string, int> extraerCiudadMinima();
    void disminuirDistancia(const std::string& ciudad, int nuevaDistancia);
    ~monticulo();
};

monticulo::~monticulo() {
    Nodo* actual = inicio;
    while (actual) {
        Nodo* siguienteNodo = actual->siguiente;
        delete actual;
        actual = siguienteNodo;
    }
}

void monticulo::intercambiarNodos(Nodo* a, Nodo* b) {
    std::swap(a->ciudad, b->ciudad);
    std::swap(a->distancia, b->distancia);
}

void monticulo::hundirNodo(Nodo* nodo) {
    while (nodo->siguiente && nodo->distancia > nodo->siguiente->distancia) {
        intercambiarNodos(nodo, nodo->siguiente);
        nodo = nodo->siguiente;
    }
}

void monticulo::elevarNodo(Nodo* nodo) {
    while (nodo->anterior && nodo->distancia < nodo->anterior->distancia) {
        intercambiarNodos(nodo, nodo->anterior);
        nodo = nodo->anterior;
    }
}

void monticulo::insertarCiudad(const std::string& ciudad, int distancia) {
    Nodo* nuevoNodo = new Nodo(ciudad, distancia);
    if (!inicio) {
        inicio = fin = nuevoNodo;
    } else {
        fin->siguiente = nuevoNodo;
        nuevoNodo->anterior = fin;
        fin = nuevoNodo;
        elevarNodo(nuevoNodo);
    }
}

std::pair<std::string, int> monticulo::extraerCiudadMinima() {
    if (!inicio) {
        return {"", std::numeric_limits<int>::max()};
    }
    Nodo* nodoMinimo = inicio;
    inicio = inicio->siguiente;
    if (inicio) {
        inicio->anterior = nullptr;
    } else {
        fin = nullptr;
    }
    std::pair<std::string, int> resultado = {nodoMinimo->ciudad, nodoMinimo->distancia};
    delete nodoMinimo;
    if (inicio) {
        hundirNodo(inicio);
    }
    return resultado;
}

void monticulo::disminuirDistancia(const std::string& ciudad, int nuevaDistancia) {
    Nodo* actual = inicio;
    while (actual && actual->ciudad != ciudad) {
        actual = actual->siguiente;
    }
    if (actual) {
        actual->distancia = nuevaDistancia;
        elevarNodo(actual);
    }
}