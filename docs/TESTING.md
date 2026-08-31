# Testing

## Tipos de Pruebas

### 1. Pruebas Unitarias

Para verificar componentes individuales:

```bash
# Compilar y verificar que no hay errores
make clean && make

# Verificar que el binario existe
ls -la bin/App

# Mostrar versión
./bin/App --version
```

### 2. Pruebas de Compilación Multiarquitectura

Verificar compatibilidad con diferentes arquitecturas:

```bash
# Para ARM64
make clean && make
file bin/App

# Cross-compile para ARM32 (si está disponible)
# make CXX=arm-linux-gnueabihf-g++
```

### 3. Pruebas de Integración

En hardware real:

1. **Test de GPIO**: Verificar que los pines cambian de estado
2. **Test de I2C**: Detectar dispositivos en el bus
3. **Test de Motores**: Movimiento lento en cada eje
4. **Test de Endstops**: Verificar lectura de fines de carrera

## Casos de Prueba

### Caso 1: Inicialización
```
Entrada: Ejecutar aplicación
Resultado: Hardware inicializado, versión mostrada
```

### Caso 2: Movimiento Simple
```
Entrada: G0 X10
Resultado: Motor X se mueve 10mm
```

### Caso 3: Homing
```
Entrada: G28
Resultado: Todos los ejes vuelven a posición home
```

### Caso 4: Secuencia Completa
```
Entrada: 
  G28
  G0 X50 Y25
  G1 X100 Y50 F1000
  G28
Resultado: Movimientos ejecutados correctamente
```

## Verificación de Memoria

- No hay fugas de memoria (RAII + unique_ptr)
- No hay accesos inválidos a memoria
- Los recursos se liberan correctamente al salir

## Limitaciones de Testing

- Las pruebas requieren hardware real
- No hay simulador de bcm2835 disponible
- Las pruebas de timing requieren osciloscopio
