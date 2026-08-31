# Uso del CNC Controller

## Inicio Rápido

1. **Instalar dependencias**:
   ```bash
   sudo ./scripts/install_deps.sh
   ```

2. **Compilar**:
   ```bash
   make
   ```

3. **Ejecutar**:
   ```bash
   sudo ./bin/App
   ```

## Comandos G-code Soportados

### Movimiento

#### G0 - Movimiento Rápido
```
G0 X100 Y50 Z5
```
Mueve los ejes a la posición especificada a máxima velocidad.

#### G1 - Movimiento Lineal
```
G1 X100 Y50 Z5 F1000
```
Movimiento interpolado a la velocidad especificada (mm/min).

#### G28 - Ir a Home
```
G28
```
Ejecuta secuencia de homing: primero Z, luego X, Y.

### Control de Motores

#### M17 - Activar Motores
```
M17
```
Activa todos los drivers de motores.

#### M18 - Desactivar Motores
```
M18
```
Desactiva todos los drivers de motores.

## Sesión Interactiva

```
$ sudo ./bin/App
========================================
  CNC Controller for Raspberry Pi
  Version: 0.1.0
  Build: Aug 30 2026 23:27:28
  Architecture: ARM64 (64-bit)
========================================
[INFO] Inicializando bcm2835...
[INFO] Hardware inicializado correctamente.
[INFO] Escriba comandos G-code (o 'quit' para salir):
CNC> G28
[HOME] Iniciando secuencia de homing...
[HOME] Homing eje Z...
[HOME] Homing eje X...
[HOME] Homing eje Y...
[HOME] Homing completado.
CNC> G0 X50 Y25
[MOVE] Movimiento rápido a (4000, 2000, 0)
CNC> G1 X100 Y50 F1500
[MOVE] Movimiento lineal a (8000, 4000, 0) F=1500
CNC> quit
[INFO] Cerrando controlador...
```

## Ejemplos de Código G

### Rectángono en XY
```
G28          ; Ir a home
G0 Z5        ; Subir Z
G0 X0 Y0     ; Posición inicial
G1 Z-1 F100  ; Bajar herramienta
G1 X50 Y0    ; Lado 1
G1 X50 Y30   ; Lado 2
G1 X0 Y30    ; Lado 3
G1 X0 Y0     ; Lado 4
G0 Z5        ; Subir herramienta
G28          ; Volver a home
```

## Visualización OLED

La pantalla OLED muestra:
- Estado actual de la máquina
- Posición de los ejes
- Progreso de operaciones
- Menús interactivos

## Solución de Problemas

### "bcm2835_init falló"
- Ejecutar con `sudo`
- Verificar permisos de `/dev/mem`

### Motores no se mueven
- Verificar conexiones STEP/DIR/ENABLE
- Confirmar que motores están activados (M17)
- Verificar alimentación VMOT

### Endstops no funcionan
- Verificar conexiones
- Comprobar lógica (NC/NO) del endstop
