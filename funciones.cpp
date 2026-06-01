#include "funciones.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

// Funcion para cargar los datos del JSON a nuestro vector de estructuras
vector<DatoSensor> leerArchivo(string nombre) {
    vector<DatoSensor> lista;
    ifstream f(nombre);
    if (!f.is_open()) return lista;

    json datos;
    f >> datos;

    // Buscamos la etiqueta captures
    if (datos.contains("captures")) {
        for (int i = 0; i < datos["captures"].size(); i++) {
            json c = datos["captures"][i];
            DatoSensor aux;
            aux.id = c["id"];
            // Convertimos el array del JSON directamente a un vector de vectores de C++
            aux.matriz = c["matrix"].get<vector<vector<int>>>();
            lista.push_back(aux);
        }
    }
    return lista;
}

// comprobar que todas las matrices son cuadradas de 16x16
bool chequearMatrices(vector<DatoSensor> lista) {
    for (int i = 0; i < lista.size(); i++) {
        // Comprobamos si tiene 16 filas
        if (lista[i].matriz.size() != 16) return false;
        for (int j = 0; j < 16; j++) {
            // Comprobamos si cada fila tiene 16 columnas
            if (lista[i].matriz[j].size() != 16) return false;
        }
    }
    return true; // Si pasa todos los filtros, la matriz es correcta
}

// estirar la matriz de 16x16 a 128x128
vector<vector<int>> escalarMatriz(vector<vector<int>> original) {
    int n_tam = 128;
    int o_tam = 16;
    // Factor de escala para mapear las posiciones nuevas con las viejas
    float escala = (float)(o_tam - 1) / (n_tam - 1);
    
    vector<vector<int>> resultado(n_tam, vector<int>(n_tam));

    for (int i = 0; i < n_tam; i++) {
        for (int j = 0; j < n_tam; j++) {
            // Calculamos la posición flotante equivalente en la matriz original
            float fila = i * escala;
            float col = j * escala;

            // Buscamos los 4 pixeles vecinos mas cercanos
            int x1 = (int)floor(col);
            int y1 = (int)floor(fila);
            int x2 = (int)ceil(col);
            int y2 = (int)ceil(fila);

            // Control de limites
            if (x2 >= o_tam) x2 = o_tam - 1;
            if (y2 >= o_tam) y2 = o_tam - 1;

            // Calculamos los pesos
            float dx = col - x1;
            float dy = fila - y1;

            // Obtenemos los valores de los 4 puntos vecinos
            int p1 = original[y1][x1];
            int p2 = original[y1][x2];
            int p3 = original[y2][x1];
            int p4 = original[y2][x2];

            // Ecuacion final 
            float valor = p1*(1-dx)*(1-dy) + p2*dx*(1-dy) + p3*(1-dx)*dy + p4*dx*dy;
            resultado[i][j] = (int)valor;
        }
    }
    return resultado;
}

// Función para enviar la matriz procesada al servidor Python usando curl local
void enviarDatos(vector<vector<int>> matriz_grande) {
    //convertimos la matriz a JSON
    json j;
    j["matrix"] = matriz_grande;
    
    //guardamos el JSON en un archivo 
    ofstream archivo_temp("temp.json");
    if (archivo_temp.is_open()) {
        archivo_temp << j.dump();
        archivo_temp.close();
    }

    // el comando curl ahora lee de forma segura desde el archivo @temp.json
    string comando = "curl -s -X POST -H \"Content-Type: application/json\" -d @temp.json http://127.0.0.1:5000/procesar";
    int resultado = system(comando.c_str());

    if(resultado != 0) {
        cout << "Fallo al enviar la matriz." << endl;
    }
    
    //borramos el archivo temporal para dejar limpio el directorio
    remove("temp.json");
}