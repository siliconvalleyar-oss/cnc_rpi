/**
 * @file cnc_controller.cpp
 * @brief Implementación de la clase Device_t para el controlador CNC.
 *
 * Contiene toda la lógica de inicialización del hardware, control de motores,
 * interpretación de código G, y gestión de movimientos.
 */

#include "cnc.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <cmath>
#include <cctype>

// Incluir bcm2835 solo si está disponible (compilación en RPi)
extern "C" {
#include <bcm2835.h>
}

namespace Device {

// ============================================================================
// Constructor y Destructor
// ============================================================================

Device_t::Device_t()
    : initialized_(false)
    , feed_rate_(DEFAULT_FEED_RATE)
    , absolute_positioning_(true) {
    // Configurar los 5 motores con sus pines correspondientes
    motors_[0] = MotorConfig{PIN_X_STEP, PIN_X_DIR, PIN_X_EN, false, DEFAULT_STEP_DELAY_US};
    motors_[1] = MotorConfig{PIN_Y_STEP, PIN_Y_DIR, PIN_Y_EN, false, DEFAULT_STEP_DELAY_US};
    motors_[2] = MotorConfig{PIN_Z_STEP, PIN_Z_DIR, PIN_Z_EN, false, DEFAULT_STEP_DELAY_US};
    motors_[3] = MotorConfig{PIN_A_STEP, PIN_A_DIR, PIN_A_EN, false, DEFAULT_STEP_DELAY_US};
    motors_[4] = MotorConfig{PIN_B_STEP, PIN_B_DIR, PIN_B_EN, false, DEFAULT_STEP_DELAY_US};
}

Device_t::~Device_t() {
    // Apagar todo el hardware de forma segura
    shutdown();
}

// ============================================================================
// Métodos públicos principales
// ============================================================================

void Device_t::run() {
    // Mostrar versión al iniciar
    printVersion();

    // Inicializar el hardware
    if (!initializeHardware()) {
        std::cerr << "[ERROR] No se pudo inicializar el hardware." << std::endl;
        return;
    }

    std::cout << "[INFO] Controlador CNC inicializado correctamente." << std::endl;
    std::cout << "[INFO] Escriba comandos G-code (o 'quit' para salir):" << std::endl;

    // Bucle principal de procesamiento
    std::string line;
    while (true) {
        std::cout << "CNC> ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        // Verificar comando de salida
        if (line == "quit" || line == "exit" || line == "q") {
            std::cout << "[INFO] Cerrando controlador..." << std::endl;
            break;
        }

        // Ignorar líneas vacías y comentarios
        if (line.empty() || line[0] == '(' || line[0] == ';') {
            continue;
        }

        // Procesar la línea de G-code
        if (!processGCodeLine(line)) {
            std::cerr << "[WARN] No se pudo procesar: " << line << std::endl;
        }
    }

    shutdown();
}

bool Device_t::initializeHardware() {
    std::cout << "[INFO] Inicializando bcm2835..." << std::endl;

    // Inicializar la librería bcm2835 (requiere permisos root)
    if (!bcm2835_init()) {
        std::cerr << "[ERROR] bcm2835_init falló. ¿Está ejecutando como root?" << std::endl;
        return false;
    }

    // Configurar pines de motor como salidas
    configureMotorPins(motors_[0]);
    configureMotorPins(motors_[1]);
    configureMotorPins(motors_[2]);
    configureMotorPins(motors_[3]);
    configureMotorPins(motors_[4]);

    // Configurar pines de endstops como entradas con pull-up
    bcm2835_gpio_fsel(PIN_ENDSTOP_X, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(PIN_ENDSTOP_X, BCM2835_GPIO_PUD_UP);
    bcm2835_gpio_fsel(PIN_ENDSTOP_Y, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(PIN_ENDSTOP_Y, BCM2835_GPIO_PUD_UP);
    bcm2835_gpio_fsel(PIN_ENDSTOP_Z, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(PIN_ENDSTOP_Z, BCM2835_GPIO_PUD_UP);

    // Desactivar todos los motores inicialmente (Enable HIGH = desactivado)
    for (int i = 0; i < MAX_MOTORS; i++) {
        enableMotor(i, false);
    }

    initialized_ = true;
    std::cout << "[INFO] Hardware inicializado correctamente." << std::endl;
    return true;
}

void Device_t::shutdown() {
    if (!initialized_) {
        return;
    }

    std::cout << "[INFO] Apagando motores y liberando recursos..." << std::endl;

    // Desactivar todos los motores
    for (int i = 0; i < MAX_MOTORS; i++) {
        enableMotor(i, false);
    }

    // Cerrar bcm2835
    bcm2835_close();
    initialized_ = false;
}

// ============================================================================
// Procesamiento de G-code
// ============================================================================

bool Device_t::processGCodeLine(const std::string& line) {
    GCodeCommand cmd = parseLine(line);

    // Ignorar líneas vacías
    if (cmd.letter == '\0') {
        return true;
    }

    std::cout << "[GCODE] " << cmd.letter << cmd.code;

    if (cmd.has_x) std::cout << " X" << cmd.x;
    if (cmd.has_y) std::cout << " Y" << cmd.y;
    if (cmd.has_z) std::cout << " Z" << cmd.z;
    if (cmd.has_a) std::cout << " A" << cmd.a;
    if (cmd.has_b) std::cout << " B" << cmd.b;
    if (cmd.has_f) std::cout << " F" << cmd.f;
    if (cmd.has_s) std::cout << " S" << cmd.s;

    std::cout << std::endl;

    // Ejecutar según el código
    switch (cmd.letter) {
        case 'G':
            switch (cmd.code) {
                case 0:
                    executeG0(cmd);
                    break;
                case 1:
                    executeG1(cmd);
                    break;
                case 28:
                    executeG28();
                    break;
                default:
                    std::cerr << "[WARN] Código G" << cmd.code << " no soportado." << std::endl;
                    return false;
            }
            break;
        case 'M':
            // Códigos M (funciones de máquina)
            switch (cmd.code) {
                case 17:
                    // Activar motores
                    for (int i = 0; i < MAX_MOTORS; i++) {
                        enableMotor(i, true);
                    }
                    std::cout << "[INFO] Motores activados." << std::endl;
                    break;
                case 18:
                    // Desactivar motores
                    for (int i = 0; i < MAX_MOTORS; i++) {
                        enableMotor(i, false);
                    }
                    std::cout << "[INFO] Motores desactivados." << std::endl;
                    break;
                default:
                    std::cerr << "[WARN] Código M" << cmd.code << " no soportado." << std::endl;
                    return false;
            }
            break;
        default:
            std::cerr << "[WARN] Comando no reconocido: " << cmd.letter << std::endl;
            return false;
    }

    return true;
}

void Device_t::executeG0(const GCodeCommand& cmd) {
    // Movimiento rápido a la posición objetivo
    Position target = current_position_;

    if (absolute_positioning_) {
        if (cmd.has_x) target.x = static_cast<int32_t>(cmd.x * 80); // 80 pasos/mm típico
        if (cmd.has_y) target.y = static_cast<int32_t>(cmd.y * 80);
        if (cmd.has_z) target.z = static_cast<int32_t>(cmd.z * 80);
        if (cmd.has_a) target.a = static_cast<int32_t>(cmd.a * 80);
        if (cmd.has_b) target.b = static_cast<int32_t>(cmd.b * 80);
    } else {
        if (cmd.has_x) target.x += static_cast<int32_t>(cmd.x * 80);
        if (cmd.has_y) target.y += static_cast<int32_t>(cmd.y * 80);
        if (cmd.has_z) target.z += static_cast<int32_t>(cmd.z * 80);
        if (cmd.has_a) target.a += static_cast<int32_t>(cmd.a * 80);
        if (cmd.has_b) target.b += static_cast<int32_t>(cmd.b * 80);
    }

    std::cout << "[MOVE] Movimiento rápido a (" << target.x << ", " << target.y << ", " << target.z << ")" << std::endl;

    // Usar velocidad rápida para G0
    linearInterpolate(target, 500); // 500us delay para movimiento rápido
    current_position_ = target;
}

void Device_t::executeG1(const GCodeCommand& cmd) {
    // Movimiento lineal interpolado a velocidad controlada
    Position target = current_position_;

    if (absolute_positioning_) {
        if (cmd.has_x) target.x = static_cast<int32_t>(cmd.x * 80);
        if (cmd.has_y) target.y = static_cast<int32_t>(cmd.y * 80);
        if (cmd.has_z) target.z = static_cast<int32_t>(cmd.z * 80);
        if (cmd.has_a) target.a = static_cast<int32_t>(cmd.a * 80);
        if (cmd.has_b) target.b = static_cast<int32_t>(cmd.b * 80);
    } else {
        if (cmd.has_x) target.x += static_cast<int32_t>(cmd.x * 80);
        if (cmd.has_y) target.y += static_cast<int32_t>(cmd.y * 80);
        if (cmd.has_z) target.z += static_cast<int32_t>(cmd.z * 80);
        if (cmd.has_a) target.a += static_cast<int32_t>(cmd.a * 80);
        if (cmd.has_b) target.b += static_cast<int32_t>(cmd.b * 80);
    }

    // Actualizar feed rate si se especificó
    if (cmd.has_f) {
        feed_rate_ = cmd.f;
    }

    // Calcular delay basado en feed rate (pasos por minuto -> us por paso)
    int delay_us = static_cast<int>(60000000.0f / (feed_rate_ * 80.0f));
    if (delay_us < 100) delay_us = 100; // Mínimo 100us

    std::cout << "[MOVE] Movimiento lineal a (" << target.x << ", " << target.y << ", " << target.z
              << ") F=" << feed_rate_ << std::endl;

    linearInterpolate(target, delay_us);
    current_position_ = target;
}

void Device_t::executeG28() {
    // Ir a posición home (mover hacia endstops)
    std::cout << "[HOME] Iniciando secuencia de homing..." << std::endl;

    // Activar motores
    for (int i = 0; i < MAX_MOTORS; i++) {
        enableMotor(i, true);
    }

    // Mover Z primero para evitar colisiones
    std::cout << "[HOME] Homing eje Z..." << std::endl;
    setMotorDirection(2, false); // Dirección hacia endstop
    while (!checkEndstop('Z')) {
        pulseStep(2);
        bcm2835_delayMicroseconds(1000);
    }

    // Homing X
    std::cout << "[HOME] Homing eje X..." << std::endl;
    setMotorDirection(0, false);
    while (!checkEndstop('X')) {
        pulseStep(0);
        bcm2835_delayMicroseconds(1000);
    }

    // Homing Y
    std::cout << "[HOME] Homing eje Y..." << std::endl;
    setMotorDirection(1, false);
    while (!checkEndstop('Y')) {
        pulseStep(1);
        bcm2835_delayMicroseconds(1000);
    }

    // Resetear posición
    current_position_ = Position();
    std::cout << "[HOME] Homing completado." << std::endl;
}

// ============================================================================
// Control de motores
// ============================================================================

void Device_t::moveMotor(int motor_index, int steps, int delay_us) {
    if (motor_index < 0 || motor_index >= MAX_MOTORS) {
        std::cerr << "[ERROR] Índice de motor inválido: " << motor_index << std::endl;
        return;
    }

    // Establecer dirección
    bool clockwise = (steps >= 0);
    setMotorDirection(motor_index, clockwise);

    // Activar motor
    enableMotor(motor_index, true);

    // Generar pulsos de paso
    int abs_steps = std::abs(steps);
    for (int i = 0; i < abs_steps; i++) {
        pulseStep(motor_index);
        bcm2835_delayMicroseconds(delay_us);
    }
}

void Device_t::setMotorDirection(int motor_index, bool clockwise) {
    if (motor_index < 0 || motor_index >= MAX_MOTORS) {
        return;
    }

    uint8_t pin = motors_[motor_index].dir_pin;
    bool inverted = motors_[motor_index].inverted;

    // Aplicar lógica de inversión si está configurada
    bool level = clockwise ? HIGH : LOW;
    if (inverted) {
        level = !level;
    }

    bcm2835_gpio_write(pin, level ? HIGH : LOW);
}

void Device_t::enableMotor(int motor_index, bool enabled) {
    if (motor_index < 0 || motor_index >= MAX_MOTORS) {
        return;
    }

    uint8_t pin = motors_[motor_index].enable_pin;

    // En drivers Pololu, Enable LOW = activo, HIGH = inactivo
    bcm2835_gpio_write(pin, enabled ? LOW : HIGH);
}

bool Device_t::checkEndstop(char axis) {
    uint8_t pin;
    switch (std::toupper(axis)) {
        case 'X': pin = PIN_ENDSTOP_X; break;
        case 'Y': pin = PIN_ENDSTOP_Y; break;
        case 'Z': pin = PIN_ENDSTOP_Z; break;
        default: return false;
    }

    // Los endstops normalmente cerrados leen LOW cuando están activados
    return (bcm2835_gpio_lev(pin) == LOW);
}

// ============================================================================
// Métodos privados
// ============================================================================

GCodeCommand Device_t::parseLine(const std::string& line) {
    GCodeCommand cmd;
    std::istringstream iss(line);
    std::string token;

    while (iss >> token) {
        if (token.empty()) continue;

        char letter = std::toupper(token[0]);
        float value = 0.0f;

        // Extraer valor numérico
        std::string num_str = token.substr(1);
        if (!num_str.empty()) {
            value = std::stof(num_str);
        }

        switch (letter) {
            case 'G':
                cmd.letter = 'G';
                cmd.code = static_cast<int>(value);
                break;
            case 'M':
                cmd.letter = 'M';
                cmd.code = static_cast<int>(value);
                break;
            case 'X':
                cmd.x = value;
                cmd.has_x = true;
                break;
            case 'Y':
                cmd.y = value;
                cmd.has_y = true;
                break;
            case 'Z':
                cmd.z = value;
                cmd.has_z = true;
                break;
            case 'A':
                cmd.a = value;
                cmd.has_a = true;
                break;
            case 'B':
                cmd.b = value;
                cmd.has_b = true;
                break;
            case 'F':
                cmd.f = value;
                cmd.has_f = true;
                break;
            case 'S':
                cmd.s = value;
                cmd.has_s = true;
                break;
            default:
                // Ignorar letras no reconocidas
                break;
        }
    }

    return cmd;
}

void Device_t::configureMotorPins(const MotorConfig& config) {
    // Configurar pin STEP como salida
    bcm2835_gpio_fsel(config.step_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(config.step_pin, LOW);

    // Configurar pin DIR como salida
    bcm2835_gpio_fsel(config.dir_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(config.dir_pin, LOW);

    // Configurar pin ENABLE como salida
    bcm2835_gpio_fsel(config.enable_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(config.enable_pin, HIGH); // Desactivado inicialmente
}

void Device_t::pulseStep(int motor_index) {
    if (motor_index < 0 || motor_index >= MAX_MOTORS) {
        return;
    }

    uint8_t pin = motors_[motor_index].step_pin;

    // Generar pulso: HIGH -> delay -> LOW
    bcm2835_gpio_write(pin, HIGH);
    bcm2835_delayMicroseconds(2); // Mínimo 1us para A4988
    bcm2835_gpio_write(pin, LOW);
}

void Device_t::linearInterpolate(const Position& target, int delay_us) {
    // Algoritmo de Bresenham multi-eje para interpolación lineal
    int32_t dx = target.x - current_position_.x;
    int32_t dy = target.y - current_position_.y;
    int32_t dz = target.z - current_position_.z;
    int32_t da = target.a - current_position_.a;
    int32_t db = target.b - current_position_.b;

    // Encontrar el eje dominante (mayor número de pasos)
    int32_t max_steps = std::max(std::abs(dx), std::abs(dy));
    max_steps = std::max(max_steps, std::abs(dz));
    max_steps = std::max(max_steps, std::abs(da));
    max_steps = std::max(max_steps, std::abs(db));

    if (max_steps == 0) {
        return; // Ya estamos en la posición objetivo
    }

    // Calcular incrementos fraccionarios para cada eje
    double x_inc = static_cast<double>(dx) / max_steps;
    double y_inc = static_cast<double>(dy) / max_steps;
    double z_inc = static_cast<double>(dz) / max_steps;
    double a_inc = static_cast<double>(da) / max_steps;
    double b_inc = static_cast<double>(db) / max_steps;

    double x_acc = 0, y_acc = 0, z_acc = 0, a_acc = 0, b_acc = 0;

    // Ejecutar interpolación
    for (int32_t i = 0; i < max_steps; i++) {
        x_acc += x_inc;
        y_acc += y_inc;
        z_acc += z_inc;
        a_acc += a_inc;
        b_acc += b_inc;

        // Generar paso para cada eje cuando el acumulador supera 0.5
        if (std::abs(x_acc) >= 0.5) {
            setMotorDirection(0, x_acc > 0);
            pulseStep(0);
            x_acc = (x_acc > 0) ? x_acc - 1 : x_acc + 1;
        }
        if (std::abs(y_acc) >= 0.5) {
            setMotorDirection(1, y_acc > 0);
            pulseStep(1);
            y_acc = (y_acc > 0) ? y_acc - 1 : y_acc + 1;
        }
        if (std::abs(z_acc) >= 0.5) {
            setMotorDirection(2, z_acc > 0);
            pulseStep(2);
            z_acc = (z_acc > 0) ? z_acc - 1 : z_acc + 1;
        }
        if (std::abs(a_acc) >= 0.5) {
            setMotorDirection(3, a_acc > 0);
            pulseStep(3);
            a_acc = (a_acc > 0) ? a_acc - 1 : a_acc + 1;
        }
        if (std::abs(b_acc) >= 0.5) {
            setMotorDirection(4, b_acc > 0);
            pulseStep(4);
            b_acc = (b_acc > 0) ? b_acc - 1 : b_acc + 1;
        }

        bcm2835_delayMicroseconds(delay_us);
    }
}

void Device_t::printVersion() const {
    std::cout << "========================================" << std::endl;
    std::cout << "  CNC Controller for Raspberry Pi" << std::endl;
    std::cout << "  Version: " << VERSION << std::endl;
    std::cout << "  Build: " << __DATE__ << " " << __TIME__ << std::endl;
    std::cout << "  Architecture: ";
#if defined(__aarch64__)
    std::cout << "ARM64 (64-bit)";
#elif defined(__arm__)
    std::cout << "ARM (32-bit)";
#else
    std::cout << "Unknown";
#endif
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
}

} // namespace Device
