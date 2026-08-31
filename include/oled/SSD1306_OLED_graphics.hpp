/**
 * @file SSD1306_OLED_graphics.hpp
 * @brief Funciones gráficas avanzadas para la pantalla OLED.
 *
 * Provee funciones para dibujar formas geométricas, círculos, bitmaps
 * y otras primitivas gráficas.
 */

#ifndef SSD1306_OLED_GRAPHICS_HPP
#define SSD1306_OLED_GRAPHICS_HPP

#include <cstdint>

/**
 * @brief Dibuja un círculo usando el algoritmo de punto medio.
 * @param x0 Centro X.
 * @param y0 Centro Y.
 * @param radius Radio del círculo.
 * @param color Color (1 = encendido, 0 = apagado).
 */
void drawCircle(int x0, int y0, int radius, uint8_t color);

/**
 * @brief Dibuja un círculo relleno.
 * @param x0 Centro X.
 * @param y0 Centro Y.
 * @param radius Radio del círculo.
 * @param color Color de relleno.
 */
void fillCircle(int x0, int y0, int radius, uint8_t color);

/**
 * @brief Dibuja un triángulo.
 * @param x0 Primer vértice X.
 * @param y0 Primer vértice Y.
 * @param x1 Segundo vértice X.
 * @param y1 Segundo vértice Y.
 * @param x2 Tercer vértice X.
 * @param y2 Tercer vértice Y.
 * @param color Color del borde.
 */
void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint8_t color);

/**
 * @brief Dibuja un triángulo relleno.
 * @param x0 Primer vértice X.
 * @param y0 Primer vértice Y.
 * @param x1 Segundo vértice X.
 * @param y1 Segundo vértice Y.
 * @param x2 Tercer vértice X.
 * @param y2 Tercer vértice Y.
 * @param color Color de relleno.
 */
void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint8_t color);

/**
 * @brief Dibuja un bitmap monocromático.
 * @param x Posición X.
 * @param y Posición Y.
 * @param bitmap Puntero a los datos del bitmap.
 * @param width Ancho del bitmap en píxeles.
 * @param height Alto del bitmap en píxeles.
 * @param color Color para los píxeles encendidos.
 */
void drawBitmap(int x, int y, const uint8_t* bitmap, int width, int height, uint8_t color);

/**
 * @brief Dibuja una línea con un grosor específico.
 * @param x0 Punto inicial X.
 * @param y0 Punto inicial Y.
 * @param x1 Punto final X.
 * @param y1 Punto final Y.
 * @param thickness Grosor de la línea.
 * @param color Color de la línea.
 */
void drawThickLine(int x0, int y0, int x1, int y1, int thickness, uint8_t color);

/**
 * @brief Dibuja un rectángulo con esquinas redondeadas.
 * @param x Esquina superior izquierda X.
 * @param y Esquina superior izquierda Y.
 * @param w Ancho.
 * @param h Alto.
 * @param radius Radio de las esquinas.
 * @param color Color del borde.
 */
void drawRoundRect(int x, int y, int w, int h, int radius, uint8_t color);

/**
 * @brief Dibuja un rectángulo relleno con esquinas redondeadas.
 * @param x Esquina superior izquierda X.
 * @param y Esquina superior izquierda Y.
 * @param w Ancho.
 * @param h Alto.
 * @param radius Radio de las esquinas.
 * @param color Color de relleno.
 */
void fillRoundRect(int x, int y, int w, int h, int radius, uint8_t color);

/**
 * @brief Scroll horizontal del contenido del display.
 * @param start_page Página inicial.
 * @param end_page Página final.
 * @param scroll true = derecha, false = izquierda.
 */
void scrollHorizontal(uint8_t start_page, uint8_t end_page, bool scroll);

/**
 * @brief Inicia scroll continuo.
 * @param start_page Página inicial.
 * @param end_page Página final.
 * @param speed Velocidad (0-7).
 */
void startScroll(uint8_t start_page, uint8_t end_page, uint8_t speed);

/**
 * @brief Detiene el scroll.
 */
void stopScroll();

#endif // SSD1306_OLED_GRAPHICS_HPP
