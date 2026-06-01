import matplotlib
# ¡ESTA LÍNEA ES LA CLAVE! Le dice a Python que guarde las fotos directamente 
# en el disco sin intentar abrir ventanas gráficas en Windows. Evita el fallo de tkinter.
matplotlib.use('Agg') 

from flask import Flask, request
import numpy as np
import matplotlib.pyplot as plt
import os

app = Flask(__name__)

# Creamos la carpeta para las imágenes si no existe
if not os.path.exists('mis_capturas'):
    os.makedirs('mis_capturas')

contador = 0

@app.route('/procesar', methods=['POST'])
def procesar():
    global contador
    
    datos = request.get_json()
    if datos and 'matrix' in datos:
        matriz = np.array(datos['matrix'])
        
        # Creamos el mapa de calor de forma limpia en memoria
        plt.figure(figsize=(5, 5))
        plt.imshow(matriz, cmap='inferno', vmin=0, vmax=1023)
        plt.title(f"Captura Numero {contador}")
        plt.colorbar(label="Presion")
        
        # Guardamos la imagen directamente en la carpeta
        nombre_foto = f"mis_capturas/foto_{contador}.png"
        plt.savefig(nombre_foto)
        plt.close() # Cerramos para liberar la memoria del ordenador
        
        print(f"He recibido la matriz {contador} y he guardado la foto.")
        contador = contador + 1
        
        return "Recibido correctamente", 200
    else:
        return "Error: No han llegado datos", 400

if __name__ == '__main__':
    print("Servidor listo y esperando al C++ en el puerto 5000...")
    app.run(host='0.0.0.0', port=5000, debug=False)