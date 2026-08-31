#!/bin/bash
# ============================================================================
# install_deps.sh - Script de instalación de dependencias para CNC Controller
#
# Instala la librería bcm2835 y las herramientas de compilación necesarias
# para compilar el proyecto en Raspberry Pi (32 y 64 bits).
#
# Uso: sudo ./scripts/install_deps.sh
# ============================================================================

set -e

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "=========================================="
echo "  Instalación de dependencias"
echo "  CNC Controller for Raspberry Pi"
echo "=========================================="
echo ""

# Verificar si se ejecuta como root
if [ "$EUID" -ne 0 ]; then
    echo -e "${RED}Este script debe ejecutarse como root (sudo).${NC}"
    echo "Uso: sudo $0"
    exit 1
fi

# Detectar arquitectura
ARCH=$(uname -m)
echo -e "${GREEN}Arquitectura detectada: $ARCH${NC}"

# ============================================================================
# Instalar herramientas de compilación
# ============================================================================
echo ""
echo "Instalando herramientas de compilación..."
apt-get update
apt-get install -y \
    build-essential \
    g++ \
    make \
    git \
    i2c-tools \
    wget \
    tar

echo -e "${GREEN}Herramientas de compilación instaladas.${NC}"

# ============================================================================
# Habilitar I2C y SPI
# ============================================================================
echo ""
echo "Habilitando interfaces I2C y SPI..."

# Habilitar I2C
if ! grep -q "^dtparam=i2c_arm=on" /boot/config.txt 2>/dev/null; then
    echo "dtparam=i2c_arm=on" >> /boot/config.txt
    echo -e "${YELLOW}I2C habilitado. Requiere reinicio.${NC}"
fi

# Habilitar SPI
if ! grep -q "^dtparam=spi=on" /boot/config.txt 2>/dev/null; then
    echo "dtparam=spi=on" >> /boot/config.txt
    echo -e "${YELLOW}SPI habilitado. Requiere reinicio.${NC}"
fi

# Cargar módulos
modprobe i2c-dev 2>/dev/null || true
modprobe spidev 2>/dev/null || true

echo -e "${GREEN}Interfaces configuradas.${NC}"

# ============================================================================
# Instalar bcm2835
# ============================================================================
echo ""
echo "Instalando librería bcm2835..."

BCM2835_VERSION="1.71"
BCM2835_URL="http://www.airspayce.com/mikem/bcm2835/bcm2835-${BCM2835_VERSION}.tar.gz"

# Descargar bcm2835
cd /tmp
if [ ! -f "bcm2835-${BCM2835_VERSION}.tar.gz" ]; then
    echo "Descargando bcm2835 v${BCM2835_VERSION}..."
    wget -q "$BCM2835_URL"
fi

# Extraer y compilar
echo "Extrayendo..."
tar -xzf "bcm2835-${BCM2835_VERSION}.tar.gz"
cd "bcm2835-${BCM2835_VERSION}"

echo "Compilando bcm2835..."
./configure
make
make install

# Actualizar cache de librerías
ldconfig

# Limpiar
cd /tmp
rm -rf "bcm2835-${BCM2835_VERSION}"*

echo -e "${GREEN} bcm2835 instalada correctamente.${NC}"

# ============================================================================
# Verificar instalación
# ============================================================================
echo ""
echo "Verificando instalación..."

# Verificar compilador
if command -v g++ &> /dev/null; then
    echo -e "${GREEN}✓ g++ instalado: $(g++ --version | head -1)${NC}"
else
    echo -e "${RED}✗ g++ no encontrado${NC}"
fi

# Verificar make
if command -v make &> /dev/null; then
    echo -e "${GREEN}✓ make instalado: $(make --version | head -1)${NC}"
else
    echo -e "${RED}✗ make no encontrado${NC}"
fi

# Verificar bcm2835
if [ -f /usr/local/lib/libbcm2835.a ]; then
    echo -e "${GREEN}✓ bcm2835 instalada${NC}"
else
    echo -e "${RED}✗ bcm2835 no encontrada${NC}"
fi

# Verificar I2C
if [ -e /dev/i2c-1 ]; then
    echo -e "${GREEN}✓ Dispositivo I2C disponible${NC}"
else
    echo -e "${YELLOW}! Dispositivo I2C no disponible (puede requerir reinicio)${NC}"
fi

# ============================================================================
# Resumen
# ============================================================================
echo ""
echo "=========================================="
echo "  Instalación completada"
echo "=========================================="
echo ""
echo "Para compilar el proyecto:"
echo "  make"
echo ""
echo "Para ejecutar:"
echo "  sudo ./bin/App"
echo ""
echo -e "${YELLOW}Nota: Si habilitó I2C o SPI, reinicie la Raspberry Pi.${NC}"
echo ""
