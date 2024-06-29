#ifndef NODOAVIONES_H
#define NODOAVIONES_H

#include <iostream>
#include <string>

using namespace std;

class nodoAviones{
    public: 
        std::string vuelo;
        std::string numero_de_registro;
        std::string modelo;
        int capacidad;
        std::string aerolinea;
        std::string ciudad_destino;
        std::string estado;
        nodoAviones *siguiente;
        nodoAviones *anterior;
        nodoAviones();
        nodoAviones(std::string vuelo, std::string numero_de_registro, std::string modelo, int capacidad, std::string aerolinea, std::string ciudad_destino, std::string estado);

};

nodoAviones::nodoAviones(string vuelo, string numero_de_registro, string modelo, int capacidad, string aerolinea, string ciudad_destino, string estado){
    this->vuelo = vuelo;
    this->numero_de_registro = numero_de_registro;
    this->modelo = modelo;
    this->capacidad = capacidad;
    this->aerolinea = aerolinea;
    this->ciudad_destino = ciudad_destino;
    this->estado = estado;
    this->siguiente = nullptr;
    this->anterior = nullptr;
}

nodoAviones::nodoAviones() {
}


#endif