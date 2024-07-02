#ifndef GRAFO_H
#define GRAFO_H

#include "ListaAdyacencia.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class Grafo {
private:
    struct Vertice {
        std::string nombre;
        ListaAdyacencia* adyacencia;
    };

    std::vector<Vertice> vertices;

    ListaAdyacencia* obtenerListaAdyacencia(const std::string& nombre) const;

public:
    ~Grafo();
    void cargarRutas();
    void generarGrafo(const std::string& nombreArchivo) const;
    void dijkstra(const std::string& origen, const std::string& destino, std::vector<int>& distancias, std::vector<std::string>& predecesores) const;
    void imprimirRuta(const std::string& origen, const std::string& destino, const std::vector<std::string>& predecesores) const;
    void recomendarRutaCorta(const std::string& origen, const std::string& destino) const;
};

Grafo::~Grafo() {
    for (auto& vertice : vertices) {
        delete vertice.adyacencia;
    }
}

ListaAdyacencia* Grafo::obtenerListaAdyacencia(const std::string& nombre) const {
    for (const auto& vertice : vertices) {
        if (vertice.nombre == nombre) {
            return vertice.adyacencia;
        }
    }
    return nullptr;
}

void Grafo::cargarRutas() {
    const std::string archivoRuta = "rutas.txt"; 
    std::ifstream archivo(archivoRuta);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo de rutas" << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string origen, destino, distanciaStr;
        std::getline(ss, origen, '/');
        std::getline(ss, destino, '/');
        std::getline(ss, distanciaStr, ';');
        int distancia = std::stoi(distanciaStr);

        ListaAdyacencia* listaOrigen = obtenerListaAdyacencia(origen);
        if (listaOrigen == nullptr) {
            vertices.push_back({origen, new ListaAdyacencia()});
            listaOrigen = vertices.back().adyacencia;
        }
        listaOrigen->insertarArista(destino, distancia);
        if (obtenerListaAdyacencia(destino) == nullptr) {
            vertices.push_back({destino, new ListaAdyacencia()});
        }
    }
}

void Grafo::generarGrafo(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo + ".dot");
    if (archivo.is_open()) {
        archivo << "digraph G {" << std::endl;
        for (const auto& vertice : vertices) {
            vertice.adyacencia->generarGrafico(archivo, vertice.nombre);
        }
        archivo << "}" << std::endl;
        archivo.close();

        std::string comando = "dot -Tpng " + nombreArchivo + ".dot -o " + nombreArchivo + ".png";
        system(comando.c_str());

        comando = "explorer " + nombreArchivo + ".png";
        system(comando.c_str());
    } else {
        std::cerr << "No se pudo abrir el archivo para la gráfica" << std::endl;
    }
}

void Grafo::dijkstra(const std::string& origen, const std::string& destino,
                     std::vector<int>& distancias, std::vector<std::string>& predecesores) const {
    std::vector<std::string> nombresVertices;
    for (const auto& vertice : vertices) {
        nombresVertices.push_back(vertice.nombre);
    }
    int n = nombresVertices.size();
    distancias.assign(n, INT_MAX);
    predecesores.assign(n, "");

    auto obtenerIndice = [&nombresVertices](const std::string& nombre) {
        for (size_t i = 0; i < nombresVertices.size(); ++i) {
            if (nombresVertices[i] == nombre) {
                return static_cast<int>(i);
            }
        }
        return -1;
    };

    int indiceOrigen = obtenerIndice(origen);
    if (indiceOrigen == -1) {
        std::cerr << "El origen no existe en el grafo." << std::endl;
        return;
    }
    distancias[indiceOrigen] = 0;

    std::vector<bool> visitado(n, false);

    for (int i = 0; i < n; ++i) {
        int u = -1;
        for (int j = 0; j < n; ++j) {
            if (!visitado[j] && (u == -1 || distancias[j] < distancias[u])) {
                u = j;
            }
        }

        if (distancias[u] == INT_MAX) break;

        visitado[u] = true;
        ListaAdyacencia* lista = obtenerListaAdyacencia(nombresVertices[u]);
        for (const auto& adyacente : lista->obtenerAdyacentes()) {
            int v = obtenerIndice(adyacente.first);
            int peso = adyacente.second;

            if (distancias[u] + peso < distancias[v]) {
                distancias[v] = distancias[u] + peso;
                predecesores[v] = nombresVertices[u];
            }
        }
    }
}

void Grafo::imprimirRuta(const std::string& origen, const std::string& destino,
                         const std::vector<std::string>& predecesores) const {
    auto obtenerIndice = [this](const std::string& nombre) {
        for (size_t i = 0; i < vertices.size(); ++i) {
            if (vertices[i].nombre == nombre) {
                return static_cast<int>(i);
            }
        }
        return -1;
    };

    int indiceDestino = obtenerIndice(destino);
    if (indiceDestino == -1 || predecesores[indiceDestino] == "") {
        std::cout << "No hay ruta desde " << origen << " hasta " << destino << std::endl;
        return;
    }

    struct NodoRuta {
        std::string ciudad;
        NodoRuta* siguiente;
        NodoRuta(const std::string& c) : ciudad(c), siguiente(nullptr) {}
    };

    NodoRuta* ruta = nullptr;

    for (int i = indiceDestino; i != -1; i = obtenerIndice(predecesores[i])) {
        NodoRuta* nuevo = new NodoRuta(vertices[i].nombre);
        nuevo->siguiente = ruta;
        ruta = nuevo;
    }

    std::cout << "Ruta más corta desde " << origen << " hasta " << destino << ":" << std::endl;
    NodoRuta* actual = ruta;
    while (actual != nullptr) {
        std::cout << actual->ciudad << " - ";
        NodoRuta* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
    std::cout << std::endl;
}

void Grafo::recomendarRutaCorta(const std::string& origen, const std::string& destino) const {
    std::vector<int> distancias;
    std::vector<std::string> predecesores;
    dijkstra(origen, destino, distancias, predecesores);
    imprimirRuta(origen, destino, predecesores);
}

#endif