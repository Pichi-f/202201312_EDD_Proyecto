#include <iostream>
#include "nodoPiloto.h"
#include "tablaHash.h"

using namespace std;

class listaSimplePiloto{
    public:
        nodoPiloto *primero;
        nodoPiloto *ultimo;
        listaSimplePiloto();
        bool estaVacia();
        void insertarFinal(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia);
        void visualizarLista();
        virtual ~listaSimplePiloto();
};

listaSimplePiloto::listaSimplePiloto(){
    primero = nullptr;
    ultimo = nullptr;
}

bool listaSimplePiloto::estaVacia(){
    return (primero == nullptr) && (ultimo == nullptr);
}

void listaSimplePiloto::insertarFinal(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia){
    nodoPiloto *nuevo = new nodoPiloto(nombre, nacionalidad, numero_de_id, vuelo, horas_de_vuelo, tipo_de_licencia);
    if (estaVacia()){
        primero = nuevo;
        ultimo = nuevo;
    } else {
        ultimo->siguiente = nuevo;
        ultimo = nuevo;
    }
}

void listaSimplePiloto::visualizarLista(){
    if (listaSimplePiloto::estaVacia()){
        cout << "La lista esta vacia" << endl;
    } else {
        string nodoDato;
        nodoPiloto *actual = primero;
        while (actual != nullptr){
            nodoDato = actual->numero_de_id;
            //cout << nodoDato << endl;
            cout << nodoDato << (actual->siguiente != nullptr ? " -> " : "\n");
            actual = actual->siguiente;
        }
    }
}

listaSimplePiloto::~listaSimplePiloto(){
}