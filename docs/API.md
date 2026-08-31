# API Reference - CNC Controller

## Namespace Device

### Device_t

Clase principal del controlador CNC.

```cpp
namespace Device {
    class Device_t;
}
```

## Métodos Públicos

### Constructor/Destructor

```cpp
Device_t();
~Device_t();
```

El constructor inicializa la configuración de pines.
El destructor apaga el hardware automáticamente.

### run()

```cpp
void run();
```

Método principal que:
1. Muestra la versión
2. Inicializa el hardware
3. Entra en el bucle de procesamiento de comandos

### initializeHardware()

```cpp
bool initializeHardware();
```

Inicializa bcm2835 y configura GPIOs.
Requiere permisos de root.

**Returns:** `true` si la inicialización fue exitosa.

### shutdown()

```cpp
void shutdown();
```

Apaga todos los motores y libera recursos.

### processGCodeLine()

```cpp
bool processGCodeLine(const std::string& line);
```

Procesa una línea de código G.

**Parameters:**
- `line`: Comando G-code

**Returns:** `true` si se procesó correctamente.

### moveMotor()

```cpp
void moveMotor(int motor_index, int steps, int delay_us);
```

Mueve un motor específico.

**Parameters:**
- `motor_index`: Índice del motor (0-4)
- `steps`: Cantidad de pasos (signo indica dirección)
- `delay_us`: Microsegundos entre pasos

### setMotorDirection()

```cpp
void setMotorDirection(int motor_index, bool clockwise);
```

Establece la dirección de rotación.

### enableMotor()

```cpp
void enableMotor(int motor_index, bool enabled);
```

Activa o desactiva un motor.

### checkEndstop()

```cpp
bool checkEndstop(char axis);
```

Verifica el estado de un fin de carrera.

**Parameters:**
- `axis`: 'X', 'Y', o 'Z'

**Returns:** `true` si el endstop está activado.

## Estructuras de Datos

### Position

```cpp
struct Position {
    int32_t x, y, z, a, b;
};
```

### MotorConfig

```cpp
struct MotorConfig {
    uint8_t step_pin;
    uint8_t dir_pin;
    uint8_t enable_pin;
    bool inverted;
    int step_delay_us;
};
```

### GCodeCommand

```cpp
struct GCodeCommand {
    char letter;
    int code;
    float x, y, z, a, b;
    float f, s;
    bool has_x, has_y, has_z, has_a, has_b;
    bool has_f, has_s;
};
```

## Constantes

```cpp
constexpr int MAX_MOTORS = 5;
constexpr int DEFAULT_STEP_DELAY_US = 1000;
constexpr float DEFAULT_FEED_RATE = 1000.0f;
```

## Código G Soportado

| Código | Descripción |
|--------|-------------|
| G0 | Movimiento rápido |
| G1 | Movimiento lineal |
| G28 | Ir a home |
| M17 | Activar motores |
| M18 | Desactivar motores |
