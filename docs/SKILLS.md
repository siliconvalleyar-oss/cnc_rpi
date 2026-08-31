# Habilidades del Sistema

## Habilidades Principales

### 1. Control de Motores Paso a Paso

El sistema controla hasta 5 motores con precisión:
- Generación de pulsos STEP configurables
- Control de dirección bidireccional
- Enable/Disable de drivers

### 2. Interpretación de G-code

Parseo y ejecución de comandos de código G:
- Movimientos rápidos y lineales
- Homing automático
- Control de funciones M

### 3. Interpolación Lineal

Algoritmo de Bresenham multi-eje:
- Movimientos coordinados precisos
- Distribución uniforme de pasos
- Aceleración controlada

### 4. Visualización OLED

Control completo de pantalla SSD1306:
- Texto con múltiples tamaños
- Gráficos vectoriales
- Menús interactivos

### 5. Gestión de Endstops

Monitoreo de límites de movimiento:
- Detección de homing
- Protección de límites
- Estado configurable

## Habilidades Técnicas

| Habilidad | Descripción |
|-----------|-------------|
| GPIO | Acceso directo a pines digitales |
| I2C | Comunicación con periféricos |
| SPI | Comunicación de alta velocidad |
| Interrupciones | Respuesta a eventos hardware |
| Timers | Control de tiempos preciso |

## Puntos de Extensión

El sistema está diseñado para ser extensible:

1. **Nuevos códigos G**: Añadir casos en `processGCodeLine()`
2. **Nuevos displays**: Implementar interfaz de display
3. **Comunicación**: Añadir módulos de comunicación
4. **Sensores**: Integrar nuevos sensores vía I2C/SPI
