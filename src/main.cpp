/**
 * @file main.cpp
 * @brief Punto de entrada de la aplicación CNC Controller.
 *
 * Crea una instancia del dispositivo CNC utilizando smart pointers
 * para gestión automática de memoria, y ejecuta el bucle principal.
 *
 * Uso:
 *   ./App              Ejecuta la aplicación normalmente
 *   ./App --version    Muestra la versión y termina
 */

#include <memory>
#include <iostream>
#include <cstring>
#include "cnc.hpp"

/**
 * @brief Función principal del programa.
 *
 * Crea un unique_ptr<Device::Device_t> y llama a run().
 * La memoria se libera automáticamente al salir del scope.
 *
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Array de argumentos.
 * @return 0 si la ejecución fue exitosa, 1 en caso de error.
 */
int main(int argc, char* argv[]) {
    // Verificar argumento --version
    if (argc > 1 && std::strcmp(argv[1], "--version") == 0) {
        std::cout << "CNC Controller App v" << VERSION << std::endl;
        return 0;
    }

    // Crear instancia del dispositivo con gestión inteligente de memoria
    auto device = std::make_unique<Device::Device_t>();

    // Ejecutar el bucle principal
    device->run();

    return 0;
}
