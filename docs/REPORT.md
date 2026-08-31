# Reporte del Proyecto

## Resumen

Proyecto de control CNC completo para Raspberry Pi, implementado en C++ con la librería bcm2835.

## Estadísticas

| Métrica | Valor |
|---------|-------|
| Archivos fuente | 10 |
| Líneas de código | ~1500 |
| Cabeceras | 8 |
| Documentación | 20+ archivos |
| Códigos G soportados | 5 |
| Ejes controlables | 5 |

## Tecnologías

- **Lenguaje**: C++17
- **Librería**: bcm2835
- **Protocolos**: GPIO, I2C
- **Compilación**: Make
- **Arquitecturas**: ARM32, ARM64

## Funcionalidades Implementadas

1. ✅ Control de 5 motores paso a paso
2. ✅ Interpretación de G-code básico
3. ✅ Interpolación lineal (Bresenham)
4. ✅ Pantalla OLED SSD1306
5. ✅ Fines de carrera
6. ✅ Sistema de homing
7. ✅ Gestión de memoria con smart pointers
8. ✅ Documentación completa

## Pruebas

- Compilación exitosa en ARM64
- Verificación de sintaxis
- Documentación revisada

## Limitaciones Conocidas

- Solo movimiento lineal (sin arcos G2/G3)
- Sin control de spindle
- Sin sistema de coordenadas de trabajo (G54-G59)
- Sin compensación de herramienta

## Próximos Pasos

Ver [ROADMAP.md](ROADMAP.md) para la planificación futura.
