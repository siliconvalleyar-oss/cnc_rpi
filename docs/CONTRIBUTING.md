# Contribuir

¡Gracias por tu interés en contribuir al CNC Controller!

## Cómo Contribuir

### 1. Fork y Clonar

```bash
git clone https://github.com/siliconvalleyar-oss/cnc_rpi.git
cd cnc_rpi
```

### 2. Crear Rama

```bash
git checkout -b feature/nueva-funcionalidad
```

### 3. Hacer Cambios

- Escribe código limpio y documentado
- Sigue el estilo existente
- Añade tests si es posible

### 4. Commit

```bash
git add .
git commit -m "Descripción clara del cambio"
```

### 5. Push y Pull Request

```bash
git push origin feature/nueva-funcionalidad
```

Luego crear un Pull Request en GitHub.

## Estándares de Código

### Estilo
- Usar `std::unique_ptr` en lugar de punteros crudos
- Documentar todas las funciones públicas
- Usar nombres descriptivos en inglés
- Comentarios en español o inglés (consistente dentro de cada archivo)

### Ejemplo

```cpp
/**
 * @brief Mueve un motor a la posición especificada.
 * @param motor_index Índice del motor (0-4).
 * @param position Posición objetivo en pasos.
 * @return true si el movimiento fue exitoso.
 */
bool moveToPosition(int motor_index, int32_t position);
```

## Tipos de Contribuciones

- **Bug fixes**: Corrección de errores
- **Features**: Nuevas funcionalidades
- **Docs**: Mejoras en documentación
- **Tests**: Añadir pruebas
- **Traducciones**: Traducir documentación

## Código de Conducta

- Respetar a todos los contribuidores
- Aceptar críticas constructivas
- Enfocarse en lo mejor para el proyecto

## Contacto

Abrir un Issue en GitHub para discutir cambios importantes antes de implementarlos.
