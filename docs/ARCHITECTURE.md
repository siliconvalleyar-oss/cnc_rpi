# Arquitectura del Sistema CNC Controller

## Visión General

El CNC Controller es una aplicación C++ que se ejecuta en Raspberry Pi para controlar máquinas CNC de hasta 5 ejes. Utiliza la librería bcm2835 para interactuar directamente con el hardware.

## Componentes Principales

### 1. Núcleo de Control (Device::Device_t)

La clase principal que gestiona:
- Inicialización del hardware
- Control de motores paso a paso
- Interpretación de G-code
- Gestión de movimientos

### 2. Intérprete de G-code

Módulo que parsea y ejecuta comandos de código G:
- **G0**: Movimiento rápido
- **G1**: Movimiento lineal interpolado
- **G28**: Ir a posición home
- **M17/M18**: Activar/Desactivar motores

### 3. Controlador de Motores

Gestión de hasta 5 motores paso a paso:
- Configuración de pines STEP/DIR/ENABLE
- Control de dirección y velocidad
- Generación de pulsos de paso

### 4. Pantalla OLED (SSD1306)

Interfaz de visualización:
- Control vía I2C
- Renderizado de texto y gráficos
- Visualización de estado del sistema

### 5. Gestión de Endstops

Monitoreo de fines de carrera:
- Detección de límites de movimiento
- Protección de homing

## Flujo de Ejecución

```
┌─────────────────────────────────────────────────────────────┐
│                        main()                               │
│  ┌──────────────────────────────────────────────────────┐  │
│  │              std::make_unique<Device_t>()             │  │
│  └──────────────────────────────────────────────────────┘  │
│                            │                                │
│                            ▼                                │
│  ┌──────────────────────────────────────────────────────┐  │
│  │                  device->run()                        │  │
│  │  ┌────────────────────────────────────────────────┐  │  │
│  │  │           initializeHardware()                  │  │  │
│  │  │  • bcm2835_init()                              │  │  │
│  │  │  • Configurar GPIO                             │  │  │
│  │  │  • Inicializar OLED                            │  │  │
│  │  └────────────────────────────────────────────────┘  │  │
│  │                         │                              │  │
│  │                         ▼                              │  │
│  │  ┌────────────────────────────────────────────────┐  │  │
│  │  │              Bucle Principal                    │  │  │
│  │  │  • Leer comando                                │  │  │
│  │  │  • Parsear G-code                              │  │  │
│  │  │  • Ejecutar movimiento                         │  │  │
│  │  │  • Actualizar display                          │  │  │
│  │  └────────────────────────────────────────────────┘  │  │
│  └──────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

## Interpolación Lineal (Bresenham)

El algoritmo de Bresenham multi-eje se utiliza para interpolación lineal precisa:

1. Calcular diferencias (dx, dy, dz, da, db)
2. Identificar eje dominante
3. Acumular errores fraccionarios
4. Generar pasos cuando el acumulador supera el umbral

## Gestión de Memoria

- Uso exclusivo de `std::unique_ptr` para gestión automática
- Sin `new/delete` explícitos
- RAII para recursos de hardware

## Arquitectura de Compilación

```
src/                    include/
├── main.cpp     ───►   ├── cnc.hpp
├── cnc_controller.cpp  └── oled/
├── oled/                  ├── SSD1306_OLED.hpp
│   ├── SSD1306_OLED.cpp   ├── SSD1306_OLED_font.hpp
│   └── ...                └── ...
        │
        ▼
    obj/src/            bin/
    ├── main.o          └── App
    ├── cnc_controller.o
    └── oled/
        └── ...
```

## Consideraciones de Diseño

1. **Portabilidad**: Compatible con ARM32 y ARM64
2. **Seguridad**: Verificación de límites en todos los accesos
3. **Eficiencia**: Interpolación optimizada sin punto flotante en bucle crítico
4. **Extensibilidad**: Arquitectura modular para añadir funcionalidades
