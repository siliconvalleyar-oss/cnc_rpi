# Arquitectura del Sistema (Español)

## Descripción General

El sistema CNC Controller está diseñado como una aplicación embebida para Raspberry Pi que controla máquinas CNC de hasta 5 ejes mediante la librería bcm2835.

## Patrones de Diseño

### RAII (Resource Acquisition Is Initialization)

Todos los recursos de hardware se gestionan mediante RAII:
- `bcm2835_init()` en el constructor
- `bcm2835_close()` en el destructor
- `std::unique_ptr` para memoria dinámica

### Namespace Device

El namespace `Device` encapsula toda la funcionalidad del controlador:
```cpp
namespace Device {
    class Device_t { ... };
}
```

## Módulos del Sistema

| Módulo | Archivo | Función |
|--------|---------|---------|
| Control principal | cnc_controller.cpp | Gestión del hardware y bucle principal |
| Interprete G-code | cnc_controller.cpp:parseLine() | Parseo de comandos G |
| Control motores | cnc_controller.cpp:moveMotor() | Generación de pulsos STEP |
| Pantalla OLED | SSD1306_OLED.cpp | Control del display SSD1306 |
| Gráficos | SSD1306_OLED_graphics.cpp | Funciones de dibujo avanzadas |
| Impresión | SSD1306_OLED_Print.cpp | Capa de impresión estilo Arduino |

## Flujo de un Comando G-code

```
Comando: G1 X100 Y50 F1500
    │
    ▼
┌─────────────────┐
│   parseLine()   │ → GCodeCommand { G, 1, X=100, Y=50, F=1500 }
└─────────────────┘
    │
    ▼
┌─────────────────┐
│  executeG1()    │ → Calcula pasos objetivo
└─────────────────┘
    │
    ▼
┌─────────────────────────┐
│  linearInterpolate()    │ → Bresenham multi-eje
│  • dx = 8000 pasos      │
│  • dy = 4000 pasos      │
│  • Genera pulsos STEP   │
└─────────────────────────┘
```
