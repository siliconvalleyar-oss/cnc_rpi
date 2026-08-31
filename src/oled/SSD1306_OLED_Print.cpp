/**
 * @file SSD1306_OLED_Print.cpp
 * @brief Implementación de la capa de impresión para OLED.
 */

#include "oled/SSD1306_OLED_Print.hpp"
#include "oled/SSD1306_OLED.hpp"
#include <cstdarg>
#include <cstring>

// Instancia global del display para las funciones de impresión
static SSD1306* print_display = nullptr;

/**
 * @brief Establece la instancia global del display para impresión.
 * @param disp Puntero al display SSD1306.
 */
void setPrintDisplay(SSD1306* disp) {
    print_display = disp;
}

// ============================================================================
// Implementación de OLED_Print
// ============================================================================

OLED_Print::OLED_Print()
    : buffer_(nullptr)
    , buffer_size_(0)
    , length_(0) {
}

void OLED_Print::init(uint16_t buffer_size) {
    if (buffer_) {
        delete[] buffer_;
    }
    buffer_size_ = buffer_size;
    buffer_ = new char[buffer_size_];
    clear();
}

uint16_t OLED_Print::write(char c) {
    if (length_ >= buffer_size_ - 1) {
        return 0;
    }
    buffer_[length_++] = c;
    buffer_[length_] = '\0';
    return 1;
}

uint16_t OLED_Print::write(const char* str) {
    if (!str) return 0;

    uint16_t written = 0;
    while (*str && length_ < buffer_size_ - 1) {
        buffer_[length_++] = *str++;
        written++;
    }
    buffer_[length_] = '\0';
    return written;
}

uint16_t OLED_Print::write(const uint8_t* data, uint16_t len) {
    if (!data) return 0;

    uint16_t written = 0;
    for (uint16_t i = 0; i < len && length_ < buffer_size_ - 1; i++) {
        buffer_[length_++] = static_cast<char>(data[i]);
        written++;
    }
    buffer_[length_] = '\0';
    return written;
}

uint16_t OLED_Print::printf(const char* format, ...) {
    if (!format || !buffer_) return 0;

    va_list args;
    va_start(args, format);

    uint16_t available = buffer_size_ - length_;
    int result = vsnprintf(buffer_ + length_, available, format, args);

    va_end(args);

    if (result < 0) return 0;
    if (static_cast<uint16_t>(result) >= available) {
        length_ = buffer_size_ - 1;
        return available - 1;
    }

    length_ += result;
    return result;
}

void OLED_Print::clear() {
    if (buffer_) {
        buffer_[0] = '\0';
    }
    length_ = 0;
}

// ============================================================================
// Funciones de utilidad
// ============================================================================

void printValue(const char* label, float value, int x, int y) {
    if (!print_display) return;

    char buf[32];
    snprintf(buf, sizeof(buf), "%s: %.2f", label, value);
    print_display->drawText(x, y, buf, 1, 1);
}

void printProgressBar(int x, int y, int width, int height, uint8_t percentage) {
    if (!print_display) return;

    // Limitar porcentaje
    if (percentage > 100) percentage = 100;

    // Dibujar borde
    print_display->drawRect(x, y, width, height, 1);

    // Calcular ancho de relleno
    int fill_width = (width - 2) * percentage / 100;
    if (fill_width > 0) {
        print_display->fillRect(x + 1, y + 1, fill_width, height - 2, 1);
    }
}

void printMenu(const char* title, const char** items, int num_items, int selected) {
    if (!print_display) return;

    print_display->clearDisplay();

    // Título
    print_display->drawText(0, 0, title, 1, 1);
    print_display->drawLine(0, 10, 127, 10, 1);

    // Items del menú
    for (int i = 0; i < num_items && i < 5; i++) {
        int y = 14 + i * 10;
        if (i == selected) {
            // Resaltar selección
            print_display->fillRect(0, y - 1, 128, 10, 1);
            print_display->drawText(4, y, ">", 1, 0);
            print_display->drawText(12, y, items[i], 1, 0);
        } else {
            print_display->drawText(12, y, items[i], 1, 1);
        }
    }

    print_display->display();
}
