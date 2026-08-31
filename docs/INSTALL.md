# Instalación

## Requisitos del Sistema

- Raspberry Pi (cualquier modelo con header GPIO de 40 pines)
- Raspberry Pi OS (32-bit o 64-bit)
- Acceso a Internet para descargar dependencias
- Permisos de root (sudo)

## Paso a Paso

### 1. Preparar el Sistema

```bash
sudo apt-get update
sudo apt-get upgrade -y
```

### 2. Clonar o Copiar el Proyecto

```bash
cd ~
git clone https://github.com/siliconvalleyar-oss/cnc_rpi.git
cd cnc_rpi
```

O copiar los archivos manualmente.

### 3. Instalar Dependencias

```bash
sudo ./scripts/install_deps.sh
```

Este script:
- Instala g++, make, git
- Descarga e instala bcm2835
- Habilita I2C y SPI
- Configura permisos

### 4. Habilitar Interfaces

Si no lo hizo el script:

```bash
sudo raspi-config
```

Navegar a:
- **Interface Options** → **I2C** → **Enable**
- **Interface Options** → **SPI** → **Enable**

### 5. Reiniciar

```bash
sudo reboot
```

### 6. Compilar

```bash
cd ~/cnc_rpi
make
```

### 7. Verificar

```bash
./bin/App --version
```

Debe mostrar:
```
CNC Controller App v0.1.0
```

### 8. Ejecutar

```bash
sudo ./bin/App
```

## Instalación del Binario (Opcional)

Para instalar el binario en el sistema:

```bash
sudo make install
```

Ahora puede ejecutar desde cualquier ubicación:
```bash
sudo cnc-controller
```

## Desinstalación

```bash
sudo make uninstall
```

## Verificar I2C

Para verificar que el OLED está conectado:

```bash
sudo i2cdetect -y 1
```

Debe mostrar un dispositivo en dirección `0x3C`.
