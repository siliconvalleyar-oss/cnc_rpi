# Hardware - CNC Controller

## Conexiones GPIO

### Drivers de Motores (Pololu A4988/DRV8825)

| Motor | STEP | DIR | ENABLE |
|-------|------|-----|--------|
| X     | GPIO 17 | GPIO 27 | GPIO 22 |
| Y     | GPIO 23 | GPIO 24 | GPIO 25 |
| Z     | GPIO 5  | GPIO 6  | GPIO 13 |
| A     | GPIO 19 | GPIO 26 | GPIO 16 |
| B     | GPIO 20 | GPIO 21 | GPIO 12 |

### Fines de Carrera (Endstops)

| Eje | Pin BCM |
|-----|---------|
| X   | GPIO 18 |
| Y   | GPIO 15 |
| Z   | GPIO 14 |

### Pantalla OLED SSD1306

| Señal | Pin Físico |
|-------|------------|
| SDA   | Pin 3 (GPIO 2) |
| SCL   | Pin 5 (GPIO 3) |
| VCC   | 3.3V |
| GND   | GND |

## Diagrama de Conexión

```
                    Raspberry Pi
                 ┌──────────────┐
                 │              │
    X_STEP  ◄────┤ GPIO 17      │
    X_DIR   ◄────┤ GPIO 27      │
    X_EN    ◄────┤ GPIO 22      │
    X_STOP  ────►┤ GPIO 18      │
                 │              │
    Y_STEP  ◄────┤ GPIO 23      │
    Y_DIR   ◄────┤ GPIO 24      │
    Y_EN    ◄────┤ GPIO 25      │
    Y_STOP  ────►┤ GPIO 15      │
                 │              │
    Z_STEP  ◄────┤ GPIO 5       │
    Z_DIR   ◄────┤ GPIO 6       │
    Z_EN    ◄────┤ GPIO 13      │
    Z_STOP  ────►┤ GPIO 14      │
                 │              │
    A_STEP  ◄────┤ GPIO 19      │
    A_DIR   ◄────┤ GPIO 26      │
    A_EN    ◄────┤ GPIO 16      │
                 │              │
    B_STEP  ◄────┤ GPIO 20      │
    B_DIR   ◄────┤ GPIO 21      │
    B_EN    ◄────┤ GPIO 12      │
                 │              │
    SDA     ◄────┤ GPIO 2 (Pin 3)
    SCL     ◄────┤ GPIO 3 (Pin 5)
                 │              │
                 └──────────────┘
```

## Drivers Pololu A4988

### Pines del Driver

```
┌─────────────────────────────┐
│         A4988               │
│  ┌─────────────────────┐   │
│  │ VMOT  ─┬─ GND       │   │  ← Alimentación motor (8-35V)
│  │ 2B    ─┼─ 2A        │   │  ← Bobina motor (polo B)
│  │ 1A    ─┼─ 1B        │   │  ← Bobina motor (polo A)
│  │ VDD   ─┼─ GND       │   │  ← Alimentación lógica (3.3-5V)
│  │ DIR   ─┼─ STEP      │   │  ← Señales de control
│  │ RST   ─┼─ SLP       │   │  ← Reset y Sleep (juntar)
│  │ MS1   ─┼─ MS2  MS3  │   │  ← Microstepping
│  │ EN    │             │   │  ← Enable (activo bajo)
│  └─────────────────────┘   │
└─────────────────────────────┘
```

### Configuración Microstepping

| MS1 | MS2 | MS3 | Resolución |
|-----|-----|-----|------------|
| Low | Low | Low | Full step |
| HIGH| Low | Low | 1/2 step |
| Low | HIGH| Low | 1/4 step |
| HIGH| HIGH| Low | 1/8 step |
| HIGH| HIGH| HIGH| 1/16 step |

## Consideraciones Eléctricas

1. **Alimentación del motor**: Fuente separada para VMOT (8-35V según motor)
2. **Límite de corriente**: Ajustar potenciómetro del driver según motor
3. **Desacoplo**: Condensadores de 100µF en VMOT cerca del driver
4. **Tierra común**: Conectar GND de RPi con GND de drivers

## Habilitar I2C

```bash
sudo raspi-config
# → Interface Options → I2C → Enable
```

Verificar:
```bash
sudo i2cdetect -y 1
```

## Habilitar SPI

```bash
sudo raspi-config
# → Interface Options → SPI → Enable
```
