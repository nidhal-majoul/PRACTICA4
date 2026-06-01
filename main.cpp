#include <iostream>
#include <vector>
#include "funciones.h"

using namespace std;

int main() {
    //Cargamos todas las capturas del fichero JSON
    vector<DatoSensor> mis_datos = leerArchivo("tactile_captures_50.json");

    // Comprobamos si la lista esta vacia 
    if (mis_datos.empty()) {
        cout << "Error: No se han podido cargar los datos del sensor" << endl;
        return 1; 
    }

    //Validamos que todas las matrices tengan el tamano correcto
    if (chequearMatrices(mis_datos)) {
        cout << "Validacion correcta." << endl;
        cout << "Iniciando el procesamiento y envio de datos..." << endl;
        
        //Bucle para procesar las 50 capturas 
        for (int i = 0; i < mis_datos.size(); i++) {
            vector<vector<int>> matriz_pequena = mis_datos[i].matriz;
            vector<vector<int>> matriz_grande = escalarMatriz(matriz_pequena);
            // Enviamos la matriz resultante al servidor de Python mediante HTTP
            enviarDatos(matriz_grande);
            cout << "Procesada y enviada la captura numero: " << i << endl;
        }
        
        cout << "Proceso finalizado. Todas las capturas han sido enviadas." << endl;
        
    } else {
        // Si alguna matriz no mide 16x16
        cout << "Error: Se han encontrado matrices con un tamano incorrecto." << endl;
        return 1;
    }

    return 0;
}