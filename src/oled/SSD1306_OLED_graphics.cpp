/**
 * @file SSD1306_OLED_graphics.cpp
 * @brief Implementación de funciones gráficas avanzadas.
 */

#include "oled/SSD1306_OLED.hpp"
#include "oled/SSD1306_OLED_graphics.hpp"
#include <algorithm>
#include <cmath>

extern "C" {
#include <bcm2835.h>
}

// Instancia global del display (debe ser inicializada en main)
static SSD1306* display = nullptr;

/**
 * @brief Establece la instancia global del display para las funciones gráficas.
 * @param disp Puntero al display SSD1306.
 */
void setGraphicsDisplay(SSD1306* disp) {
    display = disp;
}

void drawCircle(int x0, int y0, int radius, uint8_t color) {
    if (!display) return;

    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        display->drawPixel(x0 + x, y0 + y, color);
        display->drawPixel(x0 + y, y0 + x, color);
        display->drawPixel(x0 - y, y0 + x, color);
        display->drawPixel(x0 - x, y0 + y, color);
        display->drawPixel(x0 - x, y0 - y, color);
        display->drawPixel(x0 - y, y0 - x, color);
        display->drawPixel(x0 + y, y0 - x, color);
        display->drawPixel(x0 + x, y0 - y, color);

        y += 1;
        err += 1 + 2 * y;
        if (2 * (err - x) + 1 > 0) {
            x -= 1;
            err += 1 - 2 * x;
        }
    }
}

void fillCircle(int x0, int y0, int radius, uint8_t color) {
    if (!display) return;

    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                display->drawPixel(x0 + x, y0 + y, color);
            }
        }
    }
}

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint8_t color) {
    if (!display) return;

    display->drawLine(x0, y0, x1, y1, color);
    display->drawLine(x1, y1, x2, y2, color);
    display->drawLine(x2, y2, x0, y0, color);
}

void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint8_t color) {
    if (!display) return;

    // Ordenar vértices por coordenada Y
    if (y0 > y1) { std::swap(x0, x1); std::swap(y0, y1); }
    if (y0 > y2) { std::swap(x0, x2); std::swap(y0, y2); }
    if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }

    int total_height = y2 - y0;
    if (total_height == 0) return;

    for (int i = 0; i < total_height; i++) {
        bool second_half = i > (y1 - y0) || y1 == y0;
        int segment_height = second_half ? (y2 - y1) : (y1 - y0);
        if (segment_height == 0) continue;

        float alpha = static_cast<float>(i) / total_height;
        float beta = static_cast<float>(i - (second_half ? (y1 - y0) : 0)) / segment_height;

        int ax = x0 + (x2 - x0) * alpha;
        int bx = second_half ? (x1 + (x2 - x1) * beta) : (x0 + (x1 - x0) * beta);

        if (ax > bx) std::swap(ax, bx);

        for (int j = ax; j <= bx; j++) {
            display->drawPixel(j, y0 + i, color);
        }
    }
}

void drawBitmap(int x, int y, const uint8_t* bitmap, int width, int height, uint8_t color) {
    if (!display || !bitmap) return;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int byteIndex = (j / 8) * width + i;
            int bitIndex = j % 8;
            if (bitmap[byteIndex] & (1 << bitIndex)) {
                display->drawPixel(x + i, y + j, color);
            }
        }
    }
}

void drawThickLine(int x0, int y0, int x1, int y1, int thickness, uint8_t color) {
    if (!display || thickness <= 1) {
        if (display) display->drawLine(x0, y0, x1, y1, color);
        return;
    }

    float dx = x1 - x0;
    float dy = y1 - y0;
    float len = sqrt(dx * dx + dy * dy);
    if (len == 0) return;

    // Perpendicular unit vector
    float px = -dy / len;
    float py = dx / len;

    int half = thickness / 2;
    int x2 = x0 + px * half;
    int y2 = y0 + py * half;
    int x3 = x0 - px * half;
    int y3 = y0 - py * half;
    int x4 = x1 + px * half;
    int y4 = y1 + py * half;
    int x5 = x1 - px * half;
    int y5 = y1 - py * half;

    fillTriangle(x2, y2, x3, y3, x4, y4, color);
    fillTriangle(x3, y3, x4, y4, x5, y5, color);
}

void drawRoundRect(int x, int y, int w, int h, int radius, uint8_t color) {
    if (!display) return;

    // Líneas rectas
    display->drawLine(x + radius, y, x + w - radius - 1, y, color);
    display->drawLine(x + radius, y + h - 1, x + w - radius - 1, y + h - 1, color);
    display->drawLine(x, y + radius, x, y + h - radius - 1, color);
    display->drawLine(x + w - 1, y + radius, x + w - 1, y + h - radius - 1, color);

    // Esquinas
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int cx = 0;
    int cy = radius;

    while (cx < cy) {
        if (f >= 0) {
            cy--;
            ddF_y += 2;
            f += ddF_y;
        }
        cx++;
        ddF_x += 2;
        f += ddF_x;

        // Cuatro esquinas
        display->drawPixel(x + radius - cx, y + radius - cy, color);
        display->drawPixel(x + radius - cy, y + radius - cx, color);
        display->drawPixel(x + w - radius + cx - 1, y + radius - cy, color);
        display->drawPixel(x + w - radius + cy - 1, y + radius - cx, color);
        display->drawPixel(x + w - radius + cx - 1, y + h - radius + cy - 1, color);
        display->drawPixel(x + w - radius + cy - 1, y + h - radius + cx - 1, color);
        display->drawPixel(x + radius - cx, y + h - radius + cy - 1, color);
        display->drawPixel(x + radius - cy, y + h - radius + cx - 1, color);
    }
}

void fillRoundRect(int x, int y, int w, int h, int radius, uint8_t color) {
    if (!display) return;

    // Relleno central
    display->fillRect(x + radius, y, w - 2 * radius, h, color);
    display->fillRect(x, y + radius, radius, h - 2 * radius, color);
    display->fillRect(x + w - radius, y + radius, radius, h - 2 * radius, color);

    // Esquinas rellenas
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int cx = 0;
    int cy = radius;

    while (cx < cy) {
        if (f >= 0) {
            cy--;
            ddF_y += 2;
            f += ddF_y;
        }
        cx++;
        ddF_x += 2;
        f += ddF_x;

        display->drawLine(x + radius - cx, y + radius - cy, x + radius - cx, y + radius, color);
        display->drawLine(x + radius - cy, y + radius - cx, x + radius - cy, y + radius, color);
        display->drawLine(x + w - radius + cx - 1, y + radius - cy, x + w - radius + cx - 1, y + radius, color);
        display->drawLine(x + w - radius + cy - 1, y + radius - cx, x + w - radius + cy - 1, y + radius, color);
        display->drawLine(x + radius - cx, y + h - radius + cy - 1, x + radius - cx, y + h - radius - 1, color);
        display->drawLine(x + radius - cy, y + h - radius + cx - 1, x + radius - cy, y + h - radius - 1, color);
        display->drawLine(x + w - radius + cx - 1, y + h - radius + cy - 1, x + w - radius + cx - 1, y + h - radius - 1, color);
        display->drawLine(x + w - radius + cy - 1, y + h - radius + cx - 1, x + w - radius + cy - 1, y + h - radius - 1, color);
    }
}

void scrollHorizontal(uint8_t start_page, uint8_t end_page, bool scroll) {
    uint8_t cmd = 0x26; // Scroll right
    if (!scroll) cmd = 0x27; // Scroll left

    uint8_t buf[8] = {0x00, 0x00, start_page, 0x00, end_page, 0x00, 0xFF, cmd};
    bcm2835_i2c_write(buf, 8);
}

void startScroll(uint8_t start_page, uint8_t end_page, uint8_t speed) {
    uint8_t buf[8] = {0x00, 0x00, start_page, speed, end_page, 0x00, 0xFF, 0x2F};
    bcm2835_i2c_write(buf, 8);
}

void stopScroll() {
    uint8_t buf[2] = {0x00, 0x2E};
    bcm2835_i2c_write(buf, 2);
}
