/**
 * @file cnc.hpp
 * @brief Clase principal del controlador CNC para Raspberry Pi.
 *
 * Esta clase gestiona el control de hasta 5 motores paso a paso con drivers Pololu,
 * la interpretación de código G, y la comunicación con periféricos como la pantalla OLED.
 * Utiliza la librería bcm2835 para el acceso a GPIO, SPI e I2C.
 *
 * @version Definida en tiempo de compilación via -DVERSION
 */

#ifndef CNC_HPP
#define CNC_HPP

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

// ============================================================================
// Definiciones de pines GPIO para drivers Pololu (A4988/DRV8825)
// ============================================================================

// Pines para motor X (eje principal)
constexpr uint8_t PIN_X_STEP = 17;
constexpr uint8_t PIN_X_DIR  = 27;
constexpr uint8_t PIN_X_EN   = 22;

// Pines para motor Y
constexpr uint8_t PIN_Y_STEP = 23;
constexpr uint8_t PIN_Y_DIR  = 24;
constexpr uint8_t PIN_Y_EN   = 25;

// Pines para motor Z
constexpr uint8_t PIN_Z_STEP = 5;
constexpr uint8_t PIN_Z_DIR  = 6;
constexpr uint8_t PIN_Z_EN   = 13;

// Pines para motor A (4to eje - rotacional)
constexpr uint8_t PIN_A_STEP = 19;
constexpr uint8_t PIN_A_DIR  = 26;
constexpr uint8_t PIN_A_EN   = 16;

// Pines para motor B (5to eje - rotacional)
constexpr uint8_t PIN_B_STEP = 20;
constexpr uint8_t PIN_B_DIR  = 21;
constexpr uint8_t PIN_B_EN   = 12;

// Pines para fines de carrera (endstops)
constexpr uint8_t PIN_ENDSTOP_X = 18;
constexpr uint8_t PIN_ENDSTOP_Y = 15;
constexpr uint8_t PIN_ENDSTOP_Z = 14;

// ============================================================================
// Constantes del sistema
// ============================================================================

constexpr int MAX_MOTORS = 5;
constexpr int DEFAULT_STEP_DELAY_US = 1000;
constexpr float DEFAULT_FEED_RATE = 1000.0f;

// ============================================================================
// Estructuras de datos
// ============================================================================

/**
 * @brief Estructura que representa una posición 3D en pasos.
 */
struct Position {
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t a;
    int32_t b;

    Position() : x(0), y(0), z(0), a(0), b(0) {}
};

/**
 * @brief Estructura para configuración de un motor.
 */
struct MotorConfig {
    uint8_t step_pin;
    uint8_t dir_pin;
    uint8_t enable_pin;
    bool    inverted;
    int     step_delay_us;
};

/**
 * @brief Estructura para almacenar un comando G-code parseado.
 */
struct GCodeCommand {
    char letter;        // G, M, T, etc.
    int code;           // Número de código
    float x, y, z, a, b; // Coordenadas
    float f;            // Velocidad de avance
    float s;            // Velocidad del spindle
    bool has_x, has_y, has_z, has_a, has_b;
    bool has_f, has_s;

    GCodeCommand() : letter('G'), code(0), x(0), y(0), z(0), a(0), b(0),
                     f(0), s(0), has_x(false), has_y(false), has_z(false),
                     has_a(false), has_b(false), has_f(false), has_s(false) {}
};

// ============================================================================
// Namespace Device
// ============================================================================

namespace Device {

/**
 * @class Device_t
 * @brief Clase principal del controlador CNC.
 *
 * Gestiona la inicialización del hardware, el control de motores paso a paso,
 * la interpretación de código G, y la interfaz con periféricos.
 */
class Device_t {
public:
    /**
     * @brief Constructor por defecto.
     * Inicializa el estado interno del dispositivo.
     */
    Device_t();

    /**
     * @brief Destructor.
     * Libera recursos y apaga el hardware de forma segura.
     */
    ~Device_t();

    /**
     * @brief Método principal de ejecución.
     *
     * Inicializa el hardware, muestra la versión, y entra en el bucle principal
     * de procesamiento de comandos G-code.
     */
    void run();

    /**
     * @brief Inicializa la librería bcm2835 y configura los pines GPIO.
     * @return true si la inicialización fue exitosa, false en caso contrario.
     */
    bool initializeHardware();

    /**
     * @brief Apaga todos los motores y libera recursos.
     */
    void shutdown();

    /**
     * @brief Procesa una línea de código G.
     * @param line Línea de texto con el comando G-code.
     * @return true si se procesó correctamente, false si hubo error.
     */
    bool processGCodeLine(const std::string& line);

    /**
     * @brief Ejecuta un comando G0 (movimiento rápido).
     * @param cmd Comando G-code parseado.
     */
    void executeG0(const GCodeCommand& cmd);

    /**
     * @brief Ejecuta un comando G1 (movimiento lineal interpolado).
     * @param cmd Comando G-code parseado.
     */
    void executeG1(const GCodeCommand& cmd);

    /**
     * @brief Ejecuta un comando G28 (ir a posición home).
     */
    void executeG28();

    /**
     * @brief Mueve un motor específico una cantidad de pasos.
     * @param motor_index Índice del motor (0-4).
     * @param steps Cantidad de pasos (positivo = adelante, negativo = atrás).
     * @param delay_us Microsegundos entre pasos.
     */
    void moveMotor(int motor_index, int steps, int delay_us);

    /**
     * @brief Establece la dirección de un motor.
     * @param motor_index Índice del motor (0-4).
     * @param clockwise Sentido horario si es true.
     */
    void setMotorDirection(int motor_index, bool clockwise);

    /**
     * @brief Activa o desactiva un motor.
     * @param motor_index Índice del motor (0-4).
     * @param enabled true para activar, false para desactivar.
     */
    void enableMotor(int motor_index, bool enabled);

    /**
     * @brief Verifica si un fin de carrera está activado.
     * @param axis Eje a verificar ('X', 'Y', 'Z').
     * @return true si el endstop está activado.
     */
    bool checkEndstop(char axis);

    /**
     * @brief Muestra la versión de la aplicación por consola.
     */
    void printVersion() const;

private:
    /**
     * @brief Parsea una línea de texto y extrae el comando G-code.
     * @param line Línea de texto a parsear.
     * @return Estructura GCodeCommand con los datos extraídos.
     */
    GCodeCommand parseLine(const std::string& line);

    /**
     * @brief Configura los pines GPIO para un motor.
     * @param config Estructura con la configuración del motor.
     */
    void configureMotorPins(const MotorConfig& config);

    /**
     * @brief Genera un pulso STEP en el pin correspondiente.
     * @param motor_index Índice del motor.
     */
    void pulseStep(int motor_index);

    /**
     * @brief Interpolación lineal de Bresenham para movimiento multi-eje.
     * @param target Posición objetivo.
     * @param delay_us Retardo entre pasos.
     */
    void linearInterpolate(const Position& target, int delay_us);

    // Estado interno
    bool initialized_;
    Position current_position_;
    MotorConfig motors_[MAX_MOTORS];
    float feed_rate_;
    bool absolute_positioning_;
};

} // namespace Device

// ============================================================================
// HMC5883L Magnetometer
// ============================================================================

/**
 * @brief Dirección I2C por defecto del HMC5883L.
 */
constexpr uint8_t HMC5883L_I2C_ADDR = 0x1E;

/**
 * @brief Registros del HMC5883L.
 */
namespace HMC5883L_REG {
    constexpr uint8_t CONFIG_A    = 0x00;
    constexpr uint8_t CONFIG_B    = 0x01;
    constexpr uint8_t MODE        = 0x02;
    constexpr uint8_t DATA_X_MSB  = 0x03;
    constexpr uint8_t DATA_X_LSB  = 0x04;
    constexpr uint8_t DATA_Z_MSB  = 0x05;
    constexpr uint8_t DATA_Z_LSB  = 0x06;
    constexpr uint8_t DATA_Y_MSB  = 0x07;
    constexpr uint8_t DATA_Y_LSB  = 0x08;
    constexpr uint8_t STATUS      = 0x09;
    constexpr uint8_t ID_A        = 0x0A;
    constexpr uint8_t ID_B        = 0x0B;
    constexpr uint8_t ID_C        = 0x0C;
}

/**
 * @class HMC5883L
 * @brief Controlador para el magnetómetro HMC5883L.
 *
 * Provee interfaz para leer campo magnético en 3 ejes
 * y calcular rumbo (heading).
 */
class HMC5883L {
public:
    /**
     * @brief Constructor por defecto.
     */
    HMC5883L();

    /**
     * @brief Destructor.
     */
    ~HMC5883L();

    /**
     * @brief Inicializa el sensor con configuración por defecto.
     * @return true si el sensor responde correctamente.
     */
    bool initialize();

    /**
     * @brief Lee los valores brutos de los 3 ejes.
     * @param x Valor X de salida.
     * @param y Valor Y de salida.
     * @param z Valor Z de salida.
     * @return true si la lectura fue exitosa.
     */
    bool readRawData(int16_t& x, int16_t& y, int16_t& z);

    /**
     * @brief Calcula el rumbo en grados desde los valores magnéticos.
     * @param x Valor X.
     * @param y Valor Y.
     * @return Rumbo en grados (0-360).
     */
    float calculateHeading(int16_t x, int16_t y);

    /**
     * @brief Verifica la identidad del sensor.
     * @return true si el sensor es HMC5883L.
     */
    bool verifyIdentity();

    /**
     * @brief Establece el modo de operación.
     * @param continuous true para medición continua.
     */
    void setMode(bool continuous);

    /**
     * @brief Configura el rango de medición.
     * @param gauss Rango en gauss (0.88, 1.3, 1.9, 2.5, 4.0, 4.7, 5.6, 8.1).
     */
    void setRange(float gauss);

private:
    /**
     * @brief Escribe un byte en un registro del sensor.
     * @param reg Dirección del registro.
     * @param value Valor a escribir.
     */
    void writeRegister(uint8_t reg, uint8_t value);

    /**
     * @brief Lee un byte de un registro del sensor.
     * @param reg Dirección del registro.
     * @return Valor leído.
     */
    uint8_t readRegister(uint8_t reg);

    /**
     * @brief Lee múltiples bytes desde el sensor.
     * @param reg Registro inicial.
     * @param buffer Buffer de salida.
     * @param len Cantidad de bytes.
     */
    void readBytes(uint8_t reg, uint8_t* buffer, uint8_t len);

    bool initialized_;
    uint8_t i2c_addr_;
    float scaling_;
};

#endif // CNC_HPP
