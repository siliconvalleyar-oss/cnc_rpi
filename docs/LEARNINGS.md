# Aprendizajes del Proyecto

## Lecciones Aprendidas

### 1. Gestión de GPIO

El acceso directo a GPIO mediante bcm2835 requiere:
- Permisos de root para `/dev/mem`
- Comprensión de registros de hardware
- Manejo cuidadoso de memoria mapeada

### 2. Control de Motores Paso a Paso

Consideraciones importantes:
- Los pulsos STEP necesitan duración mínima (1µs para A4988)
- La dirección debe establecerse antes del pulso
- El enable es activo bajo en la mayoría de drivers

### 3. Interpolación Lineal

El algoritmo de Bresenham:
- Funciona con enteros, evitando punto flotante
- Distribuye pasos uniformemente entre ejes
- Es determinista y predecible

### 4. Almacenamiento de Estado

Es crucial mantener:
- Posición actual en pasos
- Estado de homing
- Configuración de feed rate
- Modo de posicionamiento (absoluto/incremental)

### 5. C++ en Entornos Embebidos

Buenas prácticas:
- `unique_ptr` para gestión automática de memoria
- RAII para recursos de hardware
- Headers organizados por funcionalidad
- Namespace para evitar colisiones

## Decisiones de Diseño

### ¿Por qué bcm2835 y no WiringPi?
- bcm2835 es más estable y mantenido
- Acceso directo a registros
- Mejor documentación de bajo nivel

### ¿Por qué C++17?
- Mejor soporte para smart pointers
- Mejoras en constexpr
- Features modernas sin overhead

### ¿Por qué no ROS?
- Overhead innecesario para control directo
- Simplicidad de despliegue
- Menor uso de recursos
