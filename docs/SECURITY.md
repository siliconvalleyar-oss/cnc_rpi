# Seguridad

## Consideraciones de Seguridad

### Seguridad Eléctrica

1. **Alimentación**: Usar fuente separada para motores. No alimentar motores desde RPi.
2. **Tierra común**: Conectar GND de RPi con GND de drivers de motores.
3. **Desacoplo**: Condensadores de 100µF mínimo en cada driver cerca de VMOT.
4. **Protección**: Diodos de protección para cargas inductivas.

### Seguridad Mecánica

1. **Endstops**: Siempre instalar fines de carrera en ambos extremos de cada eje.
2. **Parada de emergencia**: Botón de parada de emergencia físicamente accesible.
3. **Límites de movimiento**: Configurar límites software además de endstops hardware.
4. **Velocidad**: Comenzar con velocidades bajas para verificar dirección.

### Seguridad del Software

1. **Permisos**: La aplicación requiere root para acceder a GPIO. Minimizar código con privilegios.
2. **Validación**: Validar todos los comandos G-code antes de ejecutar.
3. **Timeouts**: Implementar timeouts para evitar movimientos infinitos.
4. **Watchdog**: Considerar watchdog de hardware para recuperación de fallos.

## Procedimientos de Emergencia

### Parada de Emergencia

1. Presionar botón de parada de emergencia físico
2. Si no responde, cortar alimentación principal
3. No intentar mover manualmente mientras hay alimentación

### Recuperación de Fallos

1. Verificar logs del sistema
2. Reiniciar aplicación
3. Ejecutar homing (G28) antes de continuar
4. Verificar conexiones eléctricas

## Mejores Prácticas

- Realizar pruebas sin herramienta primero
- Usar gafas de seguridad
- Mantener área de trabajo despejada
- No dejar la máquina sin supervisión durante operación
