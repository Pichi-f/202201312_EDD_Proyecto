#include <iostream>
#include <string>

using namespace std;

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

void menuReomendarRuta(){
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
                break;
            case 2:
                cout << "Carga de pilotos" << endl;
                break;
            case 3:
                cout << "Carga de rutas" << endl;
                break;
            case 4:
                cout << "Carga de movimientos" << endl;
                break;
            case 5:
                cout << "Consulta de horas de vuelo (Pilotos)" << endl;
                break;
            case 6:
                cout << "Recomendar ruta" << endl;
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
                        break;
                    case 2:
                        cout << "Inorden" << endl;
                        break;
                    case 3:
                        cout << "Postorden" << endl;
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
            case 7:
                cout << "Visualizar reportes" << endl;
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