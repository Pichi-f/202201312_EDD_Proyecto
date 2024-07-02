#ifndef TABLAHASH_H
#define TABLAHASH_H

#include <iostream>
#include "listaSimplePiloto.h"
#include <sstream>
#include <fstream>

using namespace std;

class tablaHash{
    public:
        int tamTabla;
        int numElementos;
        listaSimplePiloto *tabla;
        tablaHash();
        int Clave(string numero_de_id);
        void insertar(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia);
        void imprimirTabla();
        void generarGraphviz();
        void eliminar(string numero_de_id);
        virtual ~tablaHash();
};

tablaHash::tablaHash(){
    tamTabla = 18;
    tabla = new listaSimplePiloto[18];
    for (int i = 0; i < tamTabla; i++){
        //cout << tabla->estaVacia() << endl;
    }
    numElementos = 0;
}

int tablaHash::Clave(string numero_de_id){
    int i;
    int suma = 0;
    suma += static_cast<int>(numero_de_id[0]);
    for (int i = 1; i < numero_de_id.length(); i++){
        suma += static_cast<int>(numero_de_id[i]) - '0';
    }
    i = (int)(suma % tamTabla);
    //cout << "Llave asignada" << i << endl;
    return i;
}

void tablaHash::insertar(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia){
    int indice = Clave(numero_de_id);
    tabla[indice].insertarFinal(nombre, nacionalidad, numero_de_id, vuelo, horas_de_vuelo, tipo_de_licencia);
}

void tablaHash::imprimirTabla(){
    for (int i = 0; i < tamTabla; i++){
        cout << "[" << i << "] -> ";
        tabla[i].visualizarLista();
    }
}

tablaHash::~tablaHash(){
}

void tablaHash::generarGraphviz() {
    std::ofstream archivo("tablaHash.dot");
    archivo << "digraph G {" << std::endl;
    archivo << "node [shape=record];" << std::endl;
    archivo << "rankdir=LR;" << std::endl;

    for (int i = tamTabla - 1; i >= 0; i--) {
        archivo << "subgraph cluster_" << i << " {" << std::endl;
        archivo << "style=filled;" << std::endl;
        archivo << "color=lightgrey;" << std::endl;
        archivo << "nodoClave" << i << " [label=\"" << i << "\"];" << std::endl; 

        if (!tabla[i].estaVacia()) {
            nodoPiloto* actual = tabla[i].primero;
            std::string nodoAnterior = "nodoClave" + std::to_string(i);
            int contador = 0;
            while (actual != nullptr) {
                std::string nombreNodoActual = "nodoDatos" + std::to_string(i) + "_" + std::to_string(contador);
                archivo << nombreNodoActual << " [label=\"" << actual->numero_de_id << "\", shape=record, style=filled, color=white];" << std::endl;
                
                archivo << nodoAnterior << " -> " << nombreNodoActual << ";" << std::endl;
                
                nodoAnterior = nombreNodoActual;
                actual = actual->siguiente;
                contador++;
            }
        }

        archivo << "}" << std::endl;
    }

    archivo << "}" << std::endl;
    archivo.close();

    system("dot -Tpng tablaHash.dot -o tablaHash.png");
}

void tablaHash::eliminar(string numero_de_id){
    int indice = Clave(numero_de_id);
    tabla[indice].eliminar(numero_de_id);
}

#endif