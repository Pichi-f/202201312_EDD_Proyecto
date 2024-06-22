#include <iostream>
#include <string>
#include <fstream>
#include "nodoPiloto.h"

using namespace std;

class pilotosABB{
    public:
        ofstream archivo;
        nodoPiloto *raiz;
        int nodoDato;
        int recorrido = 0;
        pilotosABB();
        bool esVacio();
        void insertar(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia);
        nodoPiloto* insertarNodo(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia, nodoPiloto *nodoPtr);
        void generarReporte();
        void imprimirNodo(nodoPiloto* nodoPtr);
        void preOrden(nodoPiloto *);
        virtual ~pilotosABB();
};

pilotosABB::pilotosABB()
{
    raiz = nullptr;
}

bool pilotosABB::esVacio()
{
    return raiz == nullptr;
}

void pilotosABB::insertar(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia)
{
    raiz = insertarNodo(nombre, nacionalidad, numero_de_id, vuelo, horas_de_vuelo, tipo_de_licencia, raiz);
}

nodoPiloto* pilotosABB::insertarNodo(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia, nodoPiloto *nodoPtr)
{
    if (nodoPtr == nullptr) {
        nodoPtr = new nodoPiloto(nombre, nacionalidad, numero_de_id, vuelo, horas_de_vuelo, tipo_de_licencia);
    } else if (horas_de_vuelo < nodoPtr->horas_de_vuelo) {
        nodoPtr->izquierdo = insertarNodo(nombre, nacionalidad, numero_de_id, vuelo, horas_de_vuelo, tipo_de_licencia, nodoPtr->izquierdo);
    } else if (horas_de_vuelo > nodoPtr->horas_de_vuelo) {
        nodoPtr->derecho = insertarNodo(nombre, nacionalidad, numero_de_id, vuelo, horas_de_vuelo, tipo_de_licencia, nodoPtr->derecho);
    } else {
        cout << "El piloto ya existe" << endl;
    }
    return nodoPtr;
}

pilotosABB::~pilotosABB()
{
}

void pilotosABB::preOrden(nodoPiloto *nodoPtr){
    if (nodoPtr == nullptr)
    {
        return;
    }
    cout << nodoPtr->horas_de_vuelo << " - ";
    preOrden(nodoPtr->izquierdo);
    preOrden(nodoPtr->derecho);
}

void pilotosABB::generarReporte(){
    if (pilotosABB::esVacio()){}
    else{
        archivo.open("pilotos.dot", ios::out);
        archivo << "digraph G {" << endl;

        imprimirNodo(raiz);

        archivo << "}" << endl;
        archivo.close();
        system("dot -Tpng pilotos.dot -o pilotos.png");
        system("pilotos.png");
    }
} 

void pilotosABB::imprimirNodo(nodoPiloto* nodoPtr){
    if (nodoPtr == nullptr){
        return;
    }
    if (nodoPtr->izquierdo != nullptr){
        nodoDato = nodoPtr->horas_de_vuelo;
        archivo << nodoDato;
        archivo << "->";
        nodoDato = nodoPtr->izquierdo->horas_de_vuelo;
        archivo << nodoDato;
        archivo << ";";
    }
    imprimirNodo(nodoPtr->izquierdo);

    if (nodoPtr->derecho != nullptr){
        nodoDato = nodoPtr->horas_de_vuelo;
        archivo << nodoDato;
        archivo << "->";
        nodoDato = nodoPtr->derecho->horas_de_vuelo;
        archivo << nodoDato;
        archivo << ";";
    }
    imprimirNodo(nodoPtr->derecho);
}
