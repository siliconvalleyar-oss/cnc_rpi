# Reglas del Proyecto

## Reglas de Código

1. **Memoria**: Usar `std::unique_ptr` para toda asignación dinámica. Nunca `new/delete` crudos.

2. **Nombres**:
   - Clases: `PascalCase` (ej: `Device_t`, `MotorController`)
   - Funciones: `camelCase` (ej: `initialize()`, `moveMotor()`)
   - Variables: `snake_case` con guión bajo al final para miembros (ej: `initialized_`)
   - Constantes: `UPPER_SNAKE_CASE` (ej: `MAX_MOTORS`)

3. **Documentación**: Todas las funciones públicas deben tener comentarios Doxygen.

4. **Idioma**: Código y comentarios en inglés o español, consistente dentro de cada archivo.

## Reglas de Compilación

1. **Warnings**: Tratar warnings como errores (`-Wall -Wextra -Wpedantic`)
2. **Estándar**: C++17 mínimo
3. **Optimización**: `-O2` para producción

## Reglas de Git

1. **Commits**: Mensajes descriptivos en español o inglés
2. **Ramas**: `main` para producción, `feature/*` para nuevas funcionalidades
3. **Pull Requests**: Requeridos para cambios en `main`

## Reglas de Hardware

1. **Seguridad**: Siempre verificar endstops antes de mover
2. **Límites**: Respetar límites físicos de la máquina
3. **Parada de emergencia**: Implementar mecanismo de parada

## Reglas de Documentación

1. **Actualización**: Documentar cambios en cada versión
2. **Claridad**: Documentación accesible para principiantes
3. **Ejemplos**: Incluir ejemplos prácticos
