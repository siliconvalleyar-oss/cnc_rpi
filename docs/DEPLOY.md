# Despliegue

## Despliegue en Raspberry Pi

### Método 1: Compilación Local

1. Copiar archivos a la Raspberry Pi
2. Instalar dependencias: `sudo ./scripts/install_deps.sh`
3. Compilar: `make`
4. Ejecutar: `sudo ./bin/App`

### Método 2: Cross-Compilación

Desde una PC con Linux:

```bash
# Instalar toolchain
sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf

# Modificar Makefile para usar cross-compiler
# CXX = arm-linux-gnueabihf-g++
```

### Método 3: Binario Precompilado

```bash
# En la RPi
sudo cp bin/App /usr/local/bin/cnc-controller
sudo chmod +x /usr/local/bin/cnc-controller
```

## Ejecución como Servicio

### Crear servicio systemd

```bash
sudo nano /etc/systemd/system/cnc-controller.service
```

```ini
[Unit]
Description=CNC Controller Service
After=network.target

[Service]
Type=simple
ExecStart=/usr/local/bin/cnc-controller
Restart=on-failure
User=root

[Install]
WantedBy=multi-user.target
```

### Habilitar servicio

```bash
sudo systemctl enable cnc-controller
sudo systemctl start cnc-controller
```

### Ver estado

```bash
sudo systemctl status cnc-controller
journalctl -u cnc-controller -f
```

## Producción

Para uso en producción:

1. **Seguridad**: Configurar permisos adecuados
2. **Logs**: Redirigir salida a archivo de log
3. **Watchdog**: Configurar watchdog de hardware
4. **Backup**: Mantener backup del sistema
