#include <iostream>
#include <string>
#include "json.hpp"
#include <fstream>
#include "pilotosABB.h"
#include "listaCircularAviones.h"
#include "tablaHash.h"
#include "ArbolB.h"

listaCircularAviones *aviones = new listaCircularAviones();
pilotosABB *pilotos = new pilotosABB();
tablaHash *tabla = new tablaHash();
ArbolB *arbol = new ArbolB(5);

using namespace std;
using json = nlohmann::json;

void menuPrincipal(){
    cout << "Menu Principal" << endl;
    cout << "1. Carga de aviones" << endl;
    cout << "2. Carga de pilotos" << endl;
    cout << "3. Carga de rutas" << endl;
    cout << "4. Carga de movimientos" << endl;
    cout << "5. Consulta de horas de vuelo (Pilotos)" << endl;
    cout << "6. Recomendar ruta" << endl;
    cout << "7. Visualizar reportes" << endl;
    cout << "8. Salir" << endl;   
}

void cargarPilotos(){
    ifstream archivo("pilotos.json");

    if (!archivo.is_open()){
        cout << "No se pudo abrir el archivo" << endl;
        return;
    }
    json j;
    archivo >> j;

    for (const auto & item : j){
        string nombre = item["nombre"];
        string nacionalidad = item["nacionalidad"];
        string numero_de_id = item["numero_de_id"];
        string vuelo = item["vuelo"];
        int horas_de_vuelo = item["horas_de_vuelo"];
        string tipo_de_licencia = item["tipo_de_licencia"];

        pilotos->insertar(nombre, nacionalidad, numero_de_id, vuelo, horas_de_vuelo, tipo_de_licencia);
        tabla->insertar(nombre, nacionalidad, numero_de_id, vuelo, horas_de_vuelo, tipo_de_licencia);
    }
}

void cargarAviones() {
    ifstream archivo("aviones.json");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo" << endl;
        return;
    }
    json j;
    archivo >> j;

    for (const auto& item : j) {
        string vuelo = item["vuelo"];
        string numero_de_registro = item["numero_de_registro"];
        string modelo = item["modelo"];
        int capacidad = item["capacidad"];
        string aerolinea = item["aerolinea"];
        string ciudad_destino = item["ciudad_destino"];
        string estado = item["estado"];

        if (estado == "Mantenimiento" || estado == "mantenimiento") {
            aviones->insertar(vuelo, numero_de_registro, modelo, capacidad, aerolinea, ciudad_destino, estado);
        }
        else {
            arbol->insertar(nodoAviones(vuelo, numero_de_registro, modelo, capacidad, aerolinea, ciudad_destino, estado));
        }
    }
}

void menuReomendarRuta(){
    cout << "" << endl;
    cout << "Seleccione el recorrido" << endl;
    cout << "1. Preorden" << endl;
    cout << "2. Inorden" << endl;
    cout << "3. Postorden" << endl;
    cout << "4. Salir" << endl;
}

int main(){
    int opcion;
    do{
        menuPrincipal();
        cout << "Ingrese una opcion: ";
        cin >> opcion;
        switch(opcion){
            case 1:
                cout << "Carga de aviones" << endl;
                cargarAviones();
                break;
            case 2:
                cout << "Carga de pilotos" << endl;
                cargarPilotos();
                break;
            case 3:
                cout << "Carga de rutas" << endl;
                break;
            case 4:
                cout << "Carga de movimientos" << endl;
                break;
            case 5:
                cout << "Consulta de horas de vuelo (Pilotos)" << endl;
                int opcionRecomendar;
                do
                {
                    menuReomendarRuta();
                    cout << "Ingrese una opcion: ";
                    cin >> opcionRecomendar;
                    switch (opcionRecomendar)
                    {
                    case 1:
                        cout << "Preorden" << endl;
                        pilotos->preOrden(pilotos->raiz);
                        cout << endl;
                        break;
                    case 2:
                        cout << "Inorden" << endl;
                        pilotos->inOrden(pilotos->raiz);
                        cout << endl;
                        break;
                    case 3:
                        cout << "Postorden" << endl;
                        pilotos->postOrden(pilotos->raiz);
                        cout << endl;
                        break;
                    case 4:
                        cout << "Saliendo..." << endl;
                        break;
                    default:
                        cout << "Opcion no valida" << endl;
                        break;
                    }
                } while (opcionRecomendar != 4);
                break;
            case 6:
                cout << "Recomendar ruta" << endl;
                break;
            case 7:
                cout << "Visualizar reportes" << endl;
                aviones->graficar("aviones");
                pilotos->generarReporte();
                tabla->imprimirTabla();
                tabla->generarGraphviz();
                arbol->graficar("arbol");
                break;
            case 8:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida" << endl;
                break;
        }
    }while(opcion != 8);

    return 0;
}