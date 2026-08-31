# Configuración Inicial

## Hardware Requerido

- Raspberry Pi (3B+, 4 o 5 recomendado)
- 5x Drivers Pololu A4988 o DRV8825
- 5x Motores paso a paso NEMA 17 o NEMA 23
- Fuente de alimentación para motor (12-24V según motores)
- Pantalla OLED SSD1306 I2C (opcional)
- 3x Fines de carrera (microswitches)
- Cables de conexión

## Conexiones Iniciales

### Paso 1: Conectar Drivers a RPi

Conectar pines STEP, DIR y ENABLE según la tabla en [HARDWARE.md](HARDWARE.md).

### Paso 2: Conectar Motores

1. Identificar pares de bobinas con multímetro
2. Conectar bobinas a A1/A2 y B1/B2 del driver
3. Ajustar límite de corriente del driver

### Paso 3: Conectar Endstops

1. Conectar endstops como normalmente cerrado (NC)
2. Conectar entre pin GPIO y GND
3. Verificar con `gpio readall`

### Paso 4: Conectar OLED

1. SDA → GPIO 2 (Pin 3)
2. SCL → GPIO 3 (Pin 5)
3. VCC → 3.3V
4. GND → GND

## Verificación

1. Instalar dependencias: `sudo ./scripts/install_deps.sh`
2. Compilar: `make`
3. Verificar OLED: `sudo i2cdetect -y 1`
4. Probar movimiento lento: `G1 X10 F100`
