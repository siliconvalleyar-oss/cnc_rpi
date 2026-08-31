#!/bin/bash
# ============================================================================
# generate_basic_src.sh - Script auxiliar para generar archivos fuente base
#
# Este script puede usarse para generar esqueletos de archivos .cpp/.hpp
# siguiendo la estructura del proyecto.
#
# Uso: ./generate_basic_src.sh [nombre_clase]
# ============================================================================

set -e

# Colores
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "=========================================="
echo "  Generador de archivos fuente base"
echo "=========================================="
echo ""

# Directorio base
SRC_DIR="src"
INC_DIR="include"

# Función para generar un archivo header
generate_header() {
    local name=$1
    local guard=$(echo "${name^^}_HPP" | tr '/' '_' | tr '.' '_')
    local filename="${INC_DIR}/${name}.hpp"

    mkdir -p "$(dirname "$filename")"

    cat > "$filename" << EOF
/**
 * @file ${name}.hpp
 * @brief Descripción de la clase ${name}.
 *
 * Generado automáticamente por generate_basic_src.sh
 */

#ifndef ${guard}
#define ${guard}

#include <cstdint>
#include <memory>

/**
 * @class ${name}
 * @brief Clase ${name}.
 *
 * Descripción detallada de la funcionalidad.
 */
class ${name} {
public:
    /**
     * @brief Constructor por defecto.
     */
    ${name}();

    /**
     * @brief Destructor.
     */
    ~${name}();

    /**
     * @brief Inicializa el componente.
     * @return true si la inicialización fue exitosa.
     */
    bool initialize();

    /**
     * @brief Método principal de ejecución.
     */
    void run();

private:
    bool initialized_;
};

#endif // ${guard}
EOF

    echo -e "${GREEN}Header creado: ${filename}${NC}"
}

# Función para generar un archivo fuente
generate_source() {
    local name=$1
    local filename="${SRC_DIR}/${name}.cpp"

    mkdir -p "$(dirname "$filename")"

    cat > "$filename" << EOF
/**
 * @file ${name}.cpp
 * @brief Implementación de la clase ${name}.
 */

#include "${name}.hpp"
#include <iostream>

${name}::${name}()
    : initialized_(false) {
}

${name}::~${name}() {
    // Limpieza de recursos
}

bool ${name}::initialize() {
    std::cout << "[INFO] Inicializando ${name}..." << std::endl;
    initialized_ = true;
    return true;
}

void ${name}::run() {
    if (!initialized_) {
        std::cerr << "[ERROR] ${name} no inicializado." << std::endl;
        return;
    }
    // Lógica principal
}
EOF

    echo -e "${GREEN}Fuente creado: ${filename}${NC}"
}

# ============================================================================
# Lógica principal
# ============================================================================

if [ $# -eq 0 ]; then
    echo "Uso: $0 <nombre_clase>"
    echo ""
    echo "Ejemplos:"
    echo "  $0 Motor"
    echo "  $0 sensors/Endstop"
    echo "  $0 engine/GCodeParser"
    echo ""
    echo "Este script genera archivos .hpp y .cpp base."
    exit 0
fi

CLASS_NAME=$1

echo "Generando archivos para: $CLASS_NAME"
echo ""

generate_header "$CLASS_NAME"
generate_source "$CLASS_NAME"

echo ""
echo "Archivos generados exitosamente."
echo "Recuerde añadirlos al Makefile si es necesario."
echo ""
