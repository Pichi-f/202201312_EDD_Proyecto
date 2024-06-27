#ifndef NODOPILOTO_H
#define NODOPILOTO_H

#include <iostream>
#include <string>

using namespace std;

class nodoPiloto{
    public:
        std::string nombre;
        std::string nacionalidad;
        std::string numero_de_id;
        std::string vuelo;
        int horas_de_vuelo;
        std::string tipo_de_licencia;
        nodoPiloto *derecho;
        nodoPiloto *izquierdo;
        nodoPiloto *anterior;
        nodoPiloto *siguiente;
        nodoPiloto(std::string nombre, std::string nacionalidad, std::string numero_de_id, std::string vuelo, int horas_de_vuelo, std::string tipo_de_licencia);
        virtual ~nodoPiloto();
};

nodoPiloto::nodoPiloto(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia){
    this->nombre = nombre;
    this->nacionalidad = nacionalidad;
    this->numero_de_id = numero_de_id;
    this->vuelo = vuelo;
    this->horas_de_vuelo = horas_de_vuelo;
    this->tipo_de_licencia = tipo_de_licencia;
    this->derecho = nullptr;
    this->izquierdo = nullptr;
    this->anterior = nullptr;
    this->siguiente = nullptr;
}

nodoPiloto::~nodoPiloto(){
}

#endif