#ifndef LISTAADYACENCIA_H
#define LISTAADYACENCIA_H

#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <iostream>

class ListaAdyacencia {
private:
    std::vector<std::pair<std::string, int>> adyacencias;

public:
    ListaAdyacencia() {}
    ~ListaAdyacencia() {}

    void insertarArista(const std::string& destino, int peso);
    void generarGrafico(std::ofstream& archivo, const std::string& verticeOrigen) const;
    std::vector<std::pair<std::string, int>> obtenerAdyacentes() const;
};

void ListaAdyacencia::insertarArista(const std::string& destino, int peso) {
    adyacencias.emplace_back(destino, peso);
}

void ListaAdyacencia::generarGrafico(std::ofstream& archivo, const std::string& verticeOrigen) const {
    for (const auto& adyacente : adyacencias) {
        archivo << "\"" << verticeOrigen << "\" -> \"" << adyacente.first << "\" [label=\"" << adyacente.second << "\"];" << std::endl;
    }
}

std::vector<std::pair<std::string, int>> ListaAdyacencia::obtenerAdyacentes() const {
    return adyacencias;
}

#endif