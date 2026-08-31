# CNC Controller for Raspberry Pi

## Descripción

Controlador CNC completo para Raspberry Pi que utiliza la librería bcm2835 para gestionar hasta 5 motores paso a paso con drivers Pololu (A4988/DRV8825), interpretar código G, y controlar una pantalla OLED SSD1306.

## Características

- **Intérprete de G-code**: Soporte para G0, G1, G28, M17, M18
- **5 ejes**: X, Y, Z, A (rotacional), B (rotacional)
- **Drivers Pololu**: Compatible con A4988 y DRV8825
- **Pantalla OLED**: SSD1306 vía I2C para visualización de estado
- **Fines de carrera**: Soporte para endstops en X, Y, Z
- **Interpolación lineal**: Algoritmo de Bresenham multi-eje
- **Multiarquitectura**: Compatible con Raspberry Pi de 32 y 64 bits

## Requisitos

- Raspberry Pi (cualquier modelo con GPIO)
- Sistema operativo Raspberry Pi OS (basado en Debian)
- Librería bcm2835
- g++ con soporte C++17
- make
- I2C habilitado (para OLED)

## Instalación de dependencias

```bash
sudo ./scripts/install_deps.sh
```

Este script instala:
- Herramientas de compilación (g++, make)
- Librería bcm2835
- Herramientas I2C
- Habilita interfaces I2C y SPI

## Compilación

```bash
make
```

El binario se generará en `bin/App`.

Para limpiar:
```bash
make clean      # Elimina archivos objeto
make distclean  # Elimina archivos objeto y binario
```

## Uso

```bash
# Ejecutar la aplicación
sudo ./bin/App

# Mostrar versión
./bin/App --version
```

### Ejemplo de sesión interactiva

```
CNC> G28          ; Ir a posición home
CNC> G0 X10 Y20   ; Movimiento rápido a X10 Y20
CNC> G1 X50 Y50 F1000  ; Movimiento lineal a velocidad 1000
CNC> M17          ; Activar motores
CNC> M18          ; Desactivar motores
CNC> quit         ; Salir
```

## Estructura del proyecto

```
├── bin/                    # Binario compilado
├── config/                 # Archivos de configuración
├── docs/                   # Documentación completa
├── examples/               # Ejemplos de código G
├── include/                # Cabeceras
│   ├── cnc.hpp            # Clase principal
│   └── oled/              # Controlador OLED
├── obj/                    # Archivos objeto
├── scripts/                # Scripts de instalación
├── src/                    # Código fuente
│   ├── main.cpp           # Punto de entrada
│   ├── cnc_controller.cpp # Implementación
│   └── oled/              # Implementación OLED
├── Makefile               # Sistema de compilación
├── VERSION                # Número de versión
└── LICENSE                # Licencia MIT
```

## Configuración de hardware

Los pines GPIO están definidos en `include/cnc.hpp`:

| Función | Pin BCM | Descripción |
|---------|---------|-------------|
| X_STEP  | GPIO 17 | Paso motor X |
| X_DIR   | GPIO 27 | Dirección motor X |
| X_EN    | GPIO 22 | Enable motor X |
| Y_STEP  | GPIO 23 | Paso motor Y |
| Y_DIR   | GPIO 24 | Dirección motor Y |
| Y_EN    | GPIO 25 | Enable motor Y |
| Z_STEP  | GPIO 5  | Paso motor Z |
| Z_DIR   | GPIO 6  | Dirección motor Z |
| Z_EN    | GPIO 13 | Enable motor Z |
| A_STEP  | GPIO 19 | Paso motor A |
| A_DIR   | GPIO 26 | Dirección motor A |
| A_EN    | GPIO 16 | Enable motor A |
| B_STEP  | GPIO 20 | Paso motor B |
| B_DIR   | GPIO 21 | Dirección motor B |
| B_EN    | GPIO 12 | Enable motor B |

## Configuración del repositorio Git

Para configurar el repositorio Git y subir a GitHub:

```bash
./scripts/setup_git.sh
```

O manualmente:

```bash
git init
git add .
git commit -m "Initial commit"
git remote add origin https://github.com/USUARIO/REPO.git
git push -u origin main
```

## Versión

La versión se muestra al iniciar la aplicación y con el argumento `--version`.

La versión actual se define en el archivo `VERSION` y se pasa como macro en compilación.

## Arquitectura

El proyecto es compatible con:
- **ARM32**: Raspberry Pi 2, 3 (32-bit OS)
- **ARM64**: Raspberry Pi 3, 4, 5 (64-bit OS)

La detección es automática mediante el Makefile.

## Licencia

Este proyecto está licenciado bajo la Licencia MIT. Ver [LICENSE](LICENSE) para más detalles.

## Contribuir

Las contribuciones son bienvenidas. Por favor, lee [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md) para más información.

## Documentación adicional

- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) - Arquitectura del sistema
- [docs/API.md](docs/API.md) - Referencia de la API
- [docs/HARDWARE.md](docs/HARDWARE.md) - Configuración de hardware
- [docs/USAGE.md](docs/USAGE.md) - Guía de uso detallada
- [docs/INSTALL.md](docs/INSTALL.md) - Instalación paso a paso
