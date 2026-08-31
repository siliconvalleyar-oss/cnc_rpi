/**
 * @file SSD1306_OLED_Print.hpp
 * @brief Capa de impresión estilo Arduino para la pantalla OLED.
 *
 * Provee una interfaz similar a Print de Arduino para facilitar
 * la escritura de texto y números en el display.
 */

#ifndef SSD1306_OLED_PRINT_HPP
#define SSD1306_OLED_PRINT_HPP

#include <cstdint>
#include <cstdio>
#include <cstring>

// ============================================================================
// Clase Print para OLED
// ============================================================================

/**
 * @class OLED_Print
 * @brief Capa de impresión compatible con interfaz Print.
 *
 * Permite escribir texto formateado usando printf-style o write().
 */
class OLED_Print {
public:
    /**
     * @brief Constructor.
     */
    OLED_Print();

    /**
     * @brief Inicializa la capa de impresión con buffer interno.
     * @param buffer_size Tamaño del buffer de texto.
     */
    void init(uint16_t buffer_size = 128);

    /**
     * @brief Escribe un carácter en el buffer.
     * @param c Carácter a escribir.
     * @return Número de bytes escritos (1 si éxito, 0 si error).
     */
    uint16_t write(char c);

    /**
     * @brief Escribe una cadena de texto.
     * @param str Cadena a escribir.
     * @return Número de bytes escritos.
     */
    uint16_t write(const char* str);

    /**
     * @brief Escribe datos binarios.
     * @param data Puntero a los datos.
     * @param len Longitud de los datos.
     * @return Número de bytes escritos.
     */
    uint16_t write(const uint8_t* data, uint16_t len);

    /**
     * @brief Imprime usando formato printf.
     * @param format Cadena de formato.
     * @param ... Argumentos variables.
     * @return Número de bytes escritos.
     */
    uint16_t printf(const char* format, ...);

    /**
     * @brief Obtiene el contenido del buffer.
     * @return Puntero al buffer interno.
     */
    const char* getBuffer() const { return buffer_; }

    /**
     * @brief Obtiene la longitud actual del contenido.
     * @return Longitud en bytes.
     */
    uint16_t getLength() const { return length_; }

    /**
     * @brief Limpia el buffer.
     */
    void clear();

private:
    char* buffer_;
    uint16_t buffer_size_;
    uint16_t length_;
};

// ============================================================================
// Funciones de utilidad para mostrar datos
// ============================================================================

/**
 * @brief Muestra un valor numérico con etiqueta en el display.
 * @param label Etiqueta descriptiva.
 * @param value Valor numérico.
 * @param x Posición X.
 * @param y Posición Y.
 */
void printValue(const char* label, float value, int x, int y);

/**
 * @brief Muestra una barra de progreso.
 * @param x Posición X.
 * @param y Posición Y.
 * @param width Ancho de la barra.
 * @param height Alto de la barra.
 * @param percentage Porcentaje (0-100).
 */
void printProgressBar(int x, int y, int width, int height, uint8_t percentage);

/**
 * @brief Muestra un menú simple.
 * @param title Título del menú.
 * @param items Array de opciones.
 * @param num_items Número de opciones.
 * @param selected Índice del elemento seleccionado.
 */
void printMenu(const char* title, const char** items, int num_items, int selected);

#endif // SSD1306_OLED_PRINT_HPP
