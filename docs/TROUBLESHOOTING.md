# Resolución de Problemas

## Problemas Comunes

### La aplicación no inicia

**Error**: `bcm2835_init falló`

**Solución**:
```bash
# Verificar permisos
sudo ./bin/App

# Verificar /dev/mem existe
ls -la /dev/mem

# Verificar módulos cargados
lsmod | grep gpio
```

### Los motores no se mueven

**Verificar**:
1. Motores están activados (comando M17)
2. Alimentación VMOT conectada
3. Conexiones STEP/DIR correctas
4. Enable del driver está bajo (activo)

**Diagnóstico**:
```bash
# Verificar estado de GPIO
gpio readall
```

### Error de compilación: bcm2835 no encontrada

```bash
# Reinstalar bcm2835
sudo ./scripts/install_deps.sh

# Verificar instalación
ls -la /usr/local/lib/libbcm2835.a
ls -la /usr/local/include/bcm2835.h
```

### La pantalla OLED no muestra nada

**Verificar**:
1. Conexiones SDA/SCL correctas
2. Alimentación 3.3V
3. Dirección I2C correcta

**Diagnóstico**:
```bash
# Detectar dispositivos I2C
sudo i2cdetect -y 1

# Debe mostrar dispositivo en 0x3C
```

### Endstops no funcionan

**Verificar**:
1. Conexión entre GPIO y GND
2. Lógica del endstop (NC vs NO)
3. Configuración de pull-up interna

**Diagnóstico**:
```python
# Test rápido con Python
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(18, GPIO.IN, pull_up_down=GPIO.PUD_UP)
print(GPIO.input(18))  # 0 = activado, 1 = libre
```

### Movimientos imprecisos

**Causas posibles**:
1. Pasos por mm incorrectos
2. Corriente del motor insuficiente
3. Velocidad demasiado alta
4. Resolución de microstepping incorrecta

**Solución**:
- Calibrar pasos/mm: pasos = (360 / paso_motor) * microstepping / paso_correa
- Ajustar potenciador de corriente del driver
- Reducir velocidad de prueba

## Obtener Ayuda

1. Revisar logs del sistema: `journalctl -xe`
2. Abrir Issue en GitHub
3. Consultar documentación en `docs/`
