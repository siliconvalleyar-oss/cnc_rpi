/**
 * @file SSD1306_OLED.hpp
 * @brief Controlador para pantalla OLED SSD1306 vía I2C.
 *
 * Provee funciones para inicializar, limpiar y mostrar texto/gráficos
 * en pantallas OLED basadas en el chip SSD1306.
 */

#ifndef SSD1306_OLED_HPP
#define SSD1306_OLED_HPP

#include <cstdint>
#include <cstring>

// ============================================================================
// Comandos SSD1306
// ============================================================================

constexpr uint8_t SSD1306_I2C_ADDR = 0x3C;
constexpr uint8_t SSD1306_CMD = 0x00;
constexpr uint8_t SSD1306_DATA = 0x40;

// Comandos de control
constexpr uint8_t SSD1306_DISPLAY_OFF = 0xAE;
constexpr uint8_t SSD1306_DISPLAY_ON = 0xAF;
constexpr uint8_t SSD1306_SET_DISPLAY_CLOCK = 0xD5;
constexpr uint8_t SSD1306_SET_MULTIPLEX = 0xA8;
constexpr uint8_t SSD1306_SET_DISPLAY_OFFSET = 0xD3;
constexpr uint8_t SSD1306_SET_START_LINE = 0x40;
constexpr uint8_t SSD1306_CHARGE_PUMP = 0x8D;
constexpr uint8_t SSD1306_SET_MEMORY_ADDR = 0x20;
constexpr uint8_t SSD1306_SET_COLUMN_ADDR = 0x21;
constexpr uint8_t SSD1306_SET_PAGE_ADDR = 0x22;
constexpr uint8_t SSD1306_SET_SEG_REMAP = 0xA1;
constexpr uint8_t SSD1306_SET_COM_OUT_DIR = 0xC8;
constexpr uint8_t SSD1306_SET_COM_PIN_CFG = 0xDA;
constexpr uint8_t SSD1306_SET_CONTRAST = 0x81;
constexpr uint8_t SSD1306_SET_PRECHARGE = 0xD9;
constexpr uint8_t SSD1306_SET_VCOM_DETECT = 0xDB;
constexpr uint8_t SSD1306_DISPLAY_ALL_ON_RESUME = 0xA4;
constexpr uint8_t SSD1306_NORMAL_DISPLAY = 0xA6;
constexpr uint8_t SSD1306_COLUMN_0 = 0x00;
constexpr uint8_t SSD1306_COLUMN_127 = 0x7F;

// ============================================================================
// Clase SSD1306
// ============================================================================

/**
 * @class SSD1306
 * @brief Controlador para pantallas OLED SSD1306.
 *
 * Gestiona la comunicación I2C con el display y provee métodos para
 * dibujar píxeles, texto y formas geométricas básicas.
 */
class SSD1306 {
public:
    /**
     * @brief Constructor.
     * @param width Ancho del display en píxeles (típicamente 128).
     * @param height Alto del display en píxeles (típicamente 64 o 32).
     */
    SSD1306(uint8_t width = 128, uint8_t height = 64);

    /**
     * @brief Destructor.
     */
    ~SSD1306();

    /**
     * @brief Inicializa el display con la configuración por defecto.
     * @return true si la inicialización fue exitosa.
     */
    bool initialize();

    /**
     * @brief Limpia todo el display (apaga todos los píxeles).
     */
    void clearDisplay();

    /**
     * @brief Muestra el buffer interno en el display.
     */
    void display();

    /**
     * @brief Establece el contraste del display.
     * @param contrast Valor de contraste (0-255).
     */
    void setContrast(uint8_t contrast);

    /**
     * @brief Invierte los colores del display.
     * @param invert true para invertir, false para normal.
     */
    void invertDisplay(bool invert);

    /**
     * @brief Enciende o apaga el display.
     * @param on true para encender, false para apagar.
     */
    void setDisplayOn(bool on);

    /**
     * @brief Dibuja un píxel en el buffer.
     * @param x Coordenada X.
     * @param y Coordenada Y.
     * @param color 1 = encendido, 0 = apagado.
     */
    void drawPixel(int x, int y, uint8_t color);

    /**
     * @brief Dibuja una línea usando el algoritmo de Bresenham.
     * @param x0 Punto inicial X.
     * @param y0 Punto inicial Y.
     * @param x1 Punto final X.
     * @param y1 Punto final Y.
     * @param color Color de la línea.
     */
    void drawLine(int x0, int y0, int x1, int y1, uint8_t color);

    /**
     * @brief Dibuja un rectángulo.
     * @param x Esquina superior izquierda X.
     * @param y Esquina superior izquierda Y.
     * @param w Ancho.
     * @param h Alto.
     * @param color Color del borde.
     */
    void drawRect(int x, int y, int w, int h, uint8_t color);

    /**
     * @brief Dibuja un rectángulo relleno.
     * @param x Esquina superior izquierda X.
     * @param y Esquina superior izquierda Y.
     * @param w Ancho.
     * @param h Alto.
     * @param color Color de relleno.
     */
    void fillRect(int x, int y, int w, int h, uint8_t color);

    /**
     * @brief Dibuja un carácter en la posición especificada.
     * @param x Posición X.
     * @param y Posición Y.
     * @param c Carácter a dibujar.
     * @param size Tamaño (1 = 8px, 2 = 16px, etc.).
     * @param color Color del texto.
     */
    void drawChar(int x, int y, char c, uint8_t size, uint8_t color);

    /**
     * @brief Escribe texto en el display.
     * @param x Posición X inicial.
     * @param y Posición Y inicial.
     * @param text Texto a escribir.
     * @param size Tamaño del texto.
     * @param color Color del texto.
     */
    void drawText(int x, int y, const char* text, uint8_t size, uint8_t color);

    /**
     * @brief Obtiene el ancho del display.
     * @return Ancho en píxeles.
     */
    uint8_t getWidth() const { return width_; }

    /**
     * @brief Obtiene el alto del display.
     * @return Alto en píxeles.
     */
    uint8_t getHeight() const { return height_; }

private:
    /**
     * @brief Envía un comando al display vía I2C.
     * @param cmd Comando a enviar.
     */
    void sendCommand(uint8_t cmd);

    /**
     * @brief Envía datos al display vía I2C.
     * @param data Puntero a los datos.
     * @param len Cantidad de bytes.
     */
    void sendData(const uint8_t* data, uint16_t len);

    uint8_t width_;
    uint8_t height_;
    uint8_t pages_;
    bool initialized_;
    uint8_t buffer_[1024]; // 128x64 / 8 = 1024 bytes
    uint8_t i2c_addr_;
};

#endif // SSD1306_OLED_HPP
