# Mapa de Memoria

## Direcciones GPIO

Las direcciones base de periféricos en Raspberry Pi:

| Periférico | Dirección Base (BCM2835) | Dirección Base (BCM2711) |
|------------|--------------------------|--------------------------|
| GPIO       | 0x20200000               | 0xFE200000               |
| PWM        | 0x2020C000               | 0xFE20C000               |
| SPI        | 0x20204000               | 0xFE204000               |
| I2C        | 0x20205000               | 0xFE205000               |
| Timer      | 0x20003000               | 0xFE003000               |

## Registros GPIO

### Selección de Función (GPFSEL)

Cada registro controla 10 pines (3 bits por pin):

| Registro | Pines | Offset |
|----------|-------|--------|
| GPFSEL0  | 0-9   | 0x00   |
| GPFSEL1  | 10-19 | 0x04   |
| GPFSEL2  | 20-29 | 0x08   |
| GPFSEL3  | 30-39 | 0x0C   |
| GPFSEL4  | 40-49 | 0x10   |
| GPFSEL5  | 50-53 | 0x14   |

### Nivel de Salida (GPSET/GPCLR)

| Registro | Función | Offset |
|----------|---------|--------|
| GPSET0   | Set pines 0-31 | 0x1C |
| GPSET1   | Set pines 32-53 | 0x20 |
| GPCLR0   | Clear pines 0-31 | 0x28 |
| GPCLR1   | Clear pines 32-53 | 0x2C |

### Nivel de Pin (GPLEV)

| Registro | Función | Offset |
|----------|---------|--------|
| GPLEV0   | Leer pines 0-31 | 0x34 |
| GPLEV1   | Leer pines 32-53 | 0x38 |

## Mapeo de Pines del Proyecto

### GPFSEL1 (GPIO 10-19)

| Bit | Pin | Función |
|-----|-----|---------|
| 21-23 | GPIO 17 | OUTPUT (X_STEP) |
| 24-26 | GPIO 18 | INPUT (X_ENDSTOP) |
| 27-29 | GPIO 19 | OUTPUT (A_STEP) |

### GPFSEL2 (GPIO 20-29)

| Bit | Pin | Función |
|-----|-----|---------|
| 0-2 | GPIO 20 | OUTPUT (B_STEP) |
| 3-5 | GPIO 21 | OUTPUT (B_DIR) |
| 6-8 | GPIO 22 | OUTPUT (X_EN) |
| 9-11 | GPIO 23 | OUTPUT (Y_STEP) |
| 12-14 | GPIO 24 | OUTPUT (Y_DIR) |
| 15-17 | GPIO 25 | OUTPUT (Y_EN) |
| 18-20 | GPIO 26 | OUTPUT (A_DIR) |
| 21-23 | GPIO 27 | OUTPUT (X_DIR) |

### GPFSEL0 (GPIO 0-9)

| Bit | Pin | Función |
|-----|-----|---------|
| 15-17 | GPIO 5 | OUTPUT (Z_STEP) |
| 18-20 | GPIO 6 | OUTPUT (Z_DIR) |

### GPFSEL1 (GPIO 10-19)

| Bit | Pin | Función |
|-----|-----|---------|
| 9-11 | GPIO 13 | OUTPUT (Z_EN) |

### GPFSEL2 (GPIO 20-29)

| Bit | Pin | Función |
|-----|-----|---------|
| 6-8 | GPIO 16 | OUTPUT (A_EN) |

## Buffer de Pantalla OLED

El buffer de framebuffer para SSD1306 (128x64):

- Tamaño: 1024 bytes (128 * 64 / 8)
- Organización: páginas de 8 filas
- 8 páginas × 128 columnas

```
Página 0: bytes 0-127    (filas 0-7)
Página 1: bytes 128-255  (filas 8-15)
...
Página 7: bytes 896-1023 (filas 56-63)
```
