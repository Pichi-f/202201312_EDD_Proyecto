#include <iostream>
#include <string>
#include "nodoAviones.h"

using namespace std;

class listaCircularAviones{
    public:
        nodoAviones *primero;
        nodoAviones *ultimo;
        int tamano;
        listaCircularAviones();
        void insertar(string vuelo, string numero_de_registro, string modelo, int capacidad, string aerolinea, string ciudad_destino, string estado);
        void mostrar();
        void graficar(string Nombre);
        nodoAviones* cambiarEstado(string estado, string numero_de_registro);
        nodoAviones* buscar(string numero_de_registro);
        void eliminar(string numero_de_registro);
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

void listaCircularAviones::graficar(string Nombre){
    string texto = "digraph G{\n";
    if (primero == nullptr && ultimo == nullptr)
    {
        cout << "No hay aviones registrados en la Lista Circular Doble" << endl;
        return;
    }
    texto += "node [shape=record];\n";
    nodoAviones *actual = primero;
    do
    {
        texto += actual->numero_de_registro + " [label = \"{Vuelo: " + actual->vuelo + " | Numero de registro: " + actual->numero_de_registro + " | Estado: " + actual->estado + "}\"];\n";
        actual = actual->siguiente;
    } while (actual != primero);
    actual = primero;
    do
    {
        texto += actual->numero_de_registro + " -> " + actual->siguiente->numero_de_registro + ";\n";
        texto += actual->numero_de_registro + " -> " + actual->anterior->numero_de_registro + ";\n";
        actual = actual->siguiente;
    } while (actual != primero);
    texto += "}";
    ofstream archivo;
    archivo.open(Nombre + ".dot", ios::out);
    archivo << texto;
    archivo.close();
    string comando = "dot -Tpng " + Nombre + ".dot -o " + Nombre + ".png";
    system(comando.c_str());

    #ifdef _WIN32
    comando = "explorer " + Nombre + ".png";
    #elif __APPLE__
    comando = "open " + Nombre + ".png &";
    #elif __linux__
    comando = "xdg-open " + Nombre + ".png &";
    #else
    #error "OS not supported!"
    #endif
    system(comando.c_str());
}

nodoAviones* listaCircularAviones::buscar(string numero_de_registro) {
    if (primero == nullptr) return nullptr;
    nodoAviones* actual = primero;
    do {
        if (actual->numero_de_registro == numero_de_registro) return actual;
        actual = actual->siguiente;
    } while (actual != primero);
    return nullptr;
}

void listaCircularAviones::eliminar(string numero_de_registro) {
    nodoAviones* nodo_a_eliminar = buscar(numero_de_registro);
    if (nodo_a_eliminar == nullptr) return;

    if (primero == ultimo && primero == nodo_a_eliminar) {
        delete primero;
        primero = ultimo = nullptr;
    } else if (nodo_a_eliminar == primero) {
        primero = primero->siguiente;
        ultimo->siguiente = primero;
        primero->anterior = ultimo;
        delete nodo_a_eliminar;
    } else if (nodo_a_eliminar == ultimo) {
        ultimo = ultimo->anterior;
        ultimo->siguiente = primero;
        primero->anterior = ultimo;
        delete nodo_a_eliminar;
    } else {
        nodo_a_eliminar->anterior->siguiente = nodo_a_eliminar->siguiente;
        nodo_a_eliminar->siguiente->anterior = nodo_a_eliminar->anterior;
        delete nodo_a_eliminar;
    }
}