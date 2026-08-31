# Changelog

## [0.1.0] - 2026-08-30

### Añadido
- Estructura base del proyecto
- Clase principal Device_t con gestión de hardware
- Intérprete de G-code básico (G0, G1, G28, M17, M18)
- Control de 5 motores paso a paso con drivers Pololu
- Implementación de pantalla OLED SSD1306
- Funciones gráficas (línea, rectángulo, círculo, triángulo)
- Soporte para fines de carrera (endstops)
- Interpolación lineal con algoritmo de Bresenham multi-eje
- Makefile con detección automática de arquitectura (ARM32/ARM64)
- Script de instalación de dependencias
- Script de configuración de Git
- Generador de archivos fuente base
- Documentación completa en español

### Arquitecturas soportadas
- ARM32: Raspberry Pi 2, 3 (32-bit)
- ARM64: Raspberry Pi 3, 4, 5 (64-bit)

### Archivos principales
- `include/cnc.hpp` - Definición de la clase principal
- `src/main.cpp` - Punto de entrada
- `src/cnc_controller.cpp` - Implementación del controlador
- `include/oled/SSD1306_OLED.hpp` - Controlador OLED
- `Makefile` - Sistema de compilación
