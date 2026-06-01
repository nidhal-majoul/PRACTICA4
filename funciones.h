#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <vector>
#include <string>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

//Estrectura para guardar cada lectura edl JSON
struct DatoSensor {
    int id;
    vector<vector<int>> matriz;
};

// Las funciones del funciones.cpp
vector<DatoSensor> leerArchivo(string nombre); 
bool chequearMatrices(vector<DatoSensor> lista);
vector<vector<int>> escalarMatriz(vector<vector<int>> original);
void enviarDatos(vector<vector<int>> matriz_grande);

#endif