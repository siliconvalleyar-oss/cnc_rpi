/**
 * @file SSD1306_OLED.cpp
 * @brief Implementación del controlador SSD1306.
 */

#include "oled/SSD1306_OLED.hpp"
#include "oled/SSD1306_OLED_font.hpp"
#include <iostream>

extern "C" {
#include <bcm2835.h>
}

SSD1306::SSD1306(uint8_t width, uint8_t height)
    : width_(width)
    , height_(height)
    , pages_(height / 8)
    , initialized_(false)
    , i2c_addr_(SSD1306_I2C_ADDR) {
    memset(buffer_, 0, sizeof(buffer_));
}

SSD1306::~SSD1306() {
    if (initialized_) {
        setDisplayOn(false);
    }
}

bool SSD1306::initialize() {
    // Secuencia de inicialización para SSD1306
    sendCommand(SSD1306_DISPLAY_OFF);
    sendCommand(SSD1306_SET_DISPLAY_CLOCK);
    sendCommand(0x80);
    sendCommand(SSD1306_SET_MULTIPLEX);
    sendCommand(height_ - 1);
    sendCommand(SSD1306_SET_DISPLAY_OFFSET);
    sendCommand(0x00);
    sendCommand(SSD1306_SET_START_LINE | 0x00);
    sendCommand(SSD1306_CHARGE_PUMP);
    sendCommand(0x14); // Enable charge pump
    sendCommand(SSD1306_SET_MEMORY_ADDR);
    sendCommand(0x00); // Horizontal addressing mode
    sendCommand(SSD1306_SET_SEG_REMAP);
    sendCommand(SSD1306_SET_COM_OUT_DIR);
    sendCommand(SSD1306_SET_COM_PIN_CFG);
    sendCommand((height_ == 64) ? 0x12 : 0x02);
    sendCommand(SSD1306_SET_CONTRAST);
    sendCommand(0xCF);
    sendCommand(SSD1306_SET_PRECHARGE);
    sendCommand(0xF1);
    sendCommand(SSD1306_SET_VCOM_DETECT);
    sendCommand(0x40);
    sendCommand(SSD1306_DISPLAY_ALL_ON_RESUME);
    sendCommand(SSD1306_NORMAL_DISPLAY);
    sendCommand(SSD1306_DISPLAY_ON);

    initialized_ = true;
    return true;
}

void SSD1306::clearDisplay() {
    memset(buffer_, 0, sizeof(buffer_));
}

void SSD1306::display() {
    // Establecer rango de columnas y páginas
    sendCommand(SSD1306_SET_COLUMN_ADDR);
    sendCommand(0);
    sendCommand(width_ - 1);
    sendCommand(SSD1306_SET_PAGE_ADDR);
    sendCommand(0);
    sendCommand(pages_ - 1);

    // Enviar buffer completo
    sendData(buffer_, width_ * pages_);
}

void SSD1306::setContrast(uint8_t contrast) {
    sendCommand(SSD1306_SET_CONTRAST);
    sendCommand(contrast);
}

void SSD1306::invertDisplay(bool invert) {
    sendCommand(invert ? 0xA7 : SSD1306_NORMAL_DISPLAY);
}

void SSD1306::setDisplayOn(bool on) {
    sendCommand(on ? SSD1306_DISPLAY_ON : SSD1306_DISPLAY_OFF);
}

void SSD1306::drawPixel(int x, int y, uint8_t color) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return;
    }

    int index = x + (y / 8) * width_;
    if (color) {
        buffer_[index] |= (1 << (y % 8));
    } else {
        buffer_[index] &= ~(1 << (y % 8));
    }
}

void SSD1306::drawLine(int x0, int y0, int x1, int y1, uint8_t color) {
    // Algoritmo de Bresenham
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        drawPixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void SSD1306::drawRect(int x, int y, int w, int h, uint8_t color) {
    drawLine(x, y, x + w - 1, y, color);
    drawLine(x, y + h - 1, x + w - 1, y + h - 1, color);
    drawLine(x, y, x, y + h - 1, color);
    drawLine(x + w - 1, y, x + w - 1, y + h - 1, color);
}

void SSD1306::fillRect(int x, int y, int w, int h, uint8_t color) {
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            drawPixel(i, j, color);
        }
    }
}

void SSD1306::drawChar(int x, int y, char c, uint8_t size, uint8_t color) {
    // Solo caracteres imprimibles ASCII
    if (c < 32 || c > 127) {
        c = '?';
    }

    int char_index = c - 32;
    int font_width = FONT_5X8[char_index][0];

    for (int i = 0; i < font_width; i++) {
        uint8_t line = FONT_5X8[char_index][i + 1];
        for (int j = 0; j < 8; j++) {
            if (line & (1 << j)) {
                if (size == 1) {
                    drawPixel(x + i, y + j, color);
                } else {
                    fillRect(x + i * size, y + j * size, size, size, color);
                }
            }
        }
    }
}

void SSD1306::drawText(int x, int y, const char* text, uint8_t size, uint8_t color) {
    int cursor_x = x;
    while (*text) {
        drawChar(cursor_x, y, *text, size, color);
        cursor_x += (FONT_5X8[*text - 32][0] + 1) * size;
        text++;
    }
}

void SSD1306::sendCommand(uint8_t cmd) {
    uint8_t buf[2] = {SSD1306_CMD, cmd};
    bcm2835_i2c_write(buf, 2);
}

void SSD1306::sendData(const uint8_t* data, uint16_t len) {
    // El SSD1306 necesita el prefijo 0x40 para datos
    // Enviamos en chunks para evitar problemas con el buffer I2C
    uint8_t temp[33]; // 1 byte prefijo + 32 bytes datos
    temp[0] = SSD1306_DATA;

    uint16_t sent = 0;
    while (sent < len) {
        uint16_t chunk = (len - sent > 32) ? 32 : (len - sent);
        memcpy(&temp[1], &data[sent], chunk);
        bcm2835_i2c_write(temp, chunk + 1);
        sent += chunk;
    }
}
