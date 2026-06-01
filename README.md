
# Práctica 4 – Procesamiento e interoperabilidad de datos de sensores táctiles

## Descripción del proyecto

Este proyecto implementa un sistema distribuido en C++ y Python diseñado para gestionar, escalar y visualizar el flujo de datos numéricos provenientes de una matriz de sensores táctiles industriales. El cliente (C++) actúa como procesador de alto rendimiento, encargándose de la ingesta de telemetría, validación estructural y ejecución manual de un algoritmo de interpolación espacial. El servidor (Python) actúa como backend analítico basado en Flask, encargado de recibir los flujos de datos estructurados y renderizar mapas cromáticos normalizados.

A diferencia de modelos basados en sockets puros, la interoperabilidad de este sistema se fundamenta en el estándar HTTP/JSON, y la comunicación se realiza de forma síncrona mediante llamadas del sistema coordinadas a través del binario nativo cURL del sistema operativo.

## Estructura del proyecto

El repositorio contiene los siguientes archivos:
* **main.cpp** → Código principal que coordina el flujo global y el bucle de las capturas (Cliente)
* **funciones.cpp** → Implementación de la lógica matemática de interpolación y el módulo de transmisión HTTP
* **funciones.h** → Archivo de cabecera con los prototipos de las funciones y estructuras dinámicas
* **json.hpp** → Librería *header-only* de nlohmann para el parseo y serialización de datos JSON
* **servidor.py** → Backend receptor en Python basado en Flask y Matplotlib (Servidor)
* **tactile_captures_50.json** → Fichero histórico de entrada que simula la telemetría del sensor físico
* **mis_capturas/** → Directorio automatizado donde se almacenan los mapas de calor .png resultantes
* **README.md** → Manual explicativo con las instrucciones de despliegue del sistema

## Funcionamiento del programa

El programa realiza las siguientes tareas de forma coordinada:
* **Lectura e Ingesta:** El programa C++ parsea el archivo histórico en memoria utilizando estructuras vectoriales dinámicas (`std::vector`).
* **Validación de Integridad:** Inspecciona y filtra cada captura para asegurar un formato bidimensional cuadrado estricto de exactamente 16x16 elementos.
* **Procesamiento Geométrico Manual:** Aplica un algoritmo de interpolación bilineal para mapear y expandir los gradientes de presión desde los 16x16 puntos originales hasta una alta resolución de 128x128 puntos de control.
* **Mecanismo de Intercambio Seguro:** Serializa la matriz grande a JSON y la vuelca transitoriamente a un archivo físico (`temp.json`) para evitar desbordamientos de búfer en los argumentos del sistema operativo.
* **Transmisión e Interoperabilidad:** Invoca al binario `curl` del núcleo para enviar el payload mediante un método HTTP POST al servidor local y destruye de inmediato el archivo intermedio llamando a `remove()`.
* **Renderizado y Almacenamiento Headless:** El servidor Flask procesa el JSON entrante, normaliza el rango dinámico de color de manera homogénea (de `0` a `1023`) bajo la paleta cromática `inferno` y exporta los mapas de calor directamente a disco en formato `.png` desactivando los hilos visuales interactivos de la interfaz de usuario mediante el backend `Agg`.

Un paquete de datos se considera **VÁLIDO** si:
* Cuenta estrictamente con una matriz cuadrada inicial de 16x16 valores de presión antes de ser procesada por el motor matemático.
## Requisitos del sistema

* Sistema operativo compatible (Windows 10/11, Linux o macOS).
* Compilador de C++ con soporte estándar (g++ recomendado).
* Intérprete de Python 3.x instalado en el path del sistema.
* Dependencias de red para Python instaladas en el entorno (pip install flask numpy matplotlib).

Herramienta cURL disponible en la consola nativa del sistema operativo de ejecución.
## Compilación del programa

Para compilar el programa cliente modular de C++:
```bash
python servidor.py
g++ main.cpp funciones.cpp -o programa
./programa 
