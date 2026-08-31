# Flujo de Trabajo

## Desarrollo

### Configuración Inicial

1. Clonar repositorio
2. Instalar dependencias: `sudo ./scripts/install_deps.sh`
3. Verificar compilación: `make`

### Ciclo de Desarrollo

```
┌─────────────────────────────────────────────────────────────────┐
│  1. Crear rama: git checkout -b feature/nueva-funcionalidad    │
│  2. Implementar cambios                                        │
│  3. Compilar: make                                             │
│  4. Probar en hardware                                         │
│  5: Commit: git commit -m "Descripción"                        │
│  6. Push: git push origin feature/...                          │
│  7. Pull Request en GitHub                                     │
└─────────────────────────────────────────────────────────────────┘
```

### Estándares de Commits

```
feat: nueva funcionalidad
fix: corrección de bug
docs: documentación
refactor: reestructuración de código
test: añadir pruebas
chore: tareas de mantenimiento
```

## Pruebas

### Pruebas en Hardware

1. **Preparación**:
   - Conectar todo el hardware
   - Verificar conexiones con multímetro
   - Comenzar sin herramienta

2. **Secuencia de Prueba**:
   ```bash
   # Compilar
   make
   
   # Ejecutar
   sudo ./bin/App
   
   # Probar comandos básicos
   CNC> M17        # Activar motores
   CNC> G28        # Homing
   CNC> G0 X10     # Movimiento lento
   CNC> M18        # Desactivar motores
   ```

3. **Verificación**:
   - Movimiento correcto en dirección esperada
   - Endstops detienen movimiento
   - Homing completa exitosamente

## Despliegue

### Instalación en Producción

```bash
# Compilar versión final
make clean && make

# Instalar en sistema
sudo make install

# Configurar como servicio
sudo systemctl enable cnc-controller
sudo systemctl start cnc-controller
```

### Monitoreo

```bash
# Ver estado del servicio
sudo systemctl status cnc-controller

# Ver logs en tiempo real
journalctl -u cnc-controller -f
```

## Mantenimiento

- Revisar logs periódicamente
- Verificar conexiones mecánicas
- Actualizar firmware cuando sea necesario
- Mantener backup del sistema
