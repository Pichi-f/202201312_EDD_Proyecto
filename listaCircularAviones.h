#include <iostream>
#include <string>
#include "nodoAviones.h"

using namespace std;

class listaCircularAviones{
    public:
        nodoAviones *primero;
        nodoAviones *ultimo;
        listaCircularAviones();
        void insertar(string vuelo, string numero_de_registro, string modelo, int capacidad, string aerolinea, string ciudad_destino, string estado);
        void mostrar();
        virtual ~listaCircularAviones();
};

listaCircularAviones::listaCircularAviones()
{
    primero = nullptr;
    ultimo = nullptr;
}

listaCircularAviones::~listaCircularAviones()
{
}

void listaCircularAviones::insertar(string vuelo, string numero_de_registro, string modelo, int capacidad, string aerolinea, string ciudad_destino, string estado)
{
    nodoAviones *nuevo = new nodoAviones(vuelo, numero_de_registro, modelo, capacidad, aerolinea, ciudad_destino, estado);
    if (primero == nullptr){
        primero = nuevo;
        ultimo = nuevo;
        primero->siguiente = primero;
        primero->anterior = ultimo;
    }
    else{
        ultimo->siguiente = nuevo;
        nuevo->anterior = ultimo;
        nuevo->siguiente = primero;
        ultimo = nuevo;
        primero->anterior = ultimo;
    }
}