/**
 * @file SSD1306_OLED_font.cpp
 * @brief Implementación de soporte de fuentes para OLED.
 *
 * Este archivo existe para mantener la estructura de compilación.
 * Las definiciones de fuentes están en SSD1306_OLED_font.hpp.
 */

#include "oled/SSD1306_OLED_font.hpp"

// Las definiciones de fuentes están en el header.
// Este archivo asegura que el archivo de cabecera compile correctamente
// y proporciona un lugar para futuras expansiones de fuentes.

namespace {
    // Verificación en tiempo de compilación del tamaño de la fuente
    static_assert(sizeof(FONT_5X8) / sizeof(FONT_5X8[0]) == 96,
                  "La fuente debe contener 96 caracteres (ASCII 32-127)");
}
