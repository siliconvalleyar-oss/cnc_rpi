#=============================================================================
# Makefile para CNC Controller - Raspberry Pi
#
# Compatible con arquitecturas ARM de 32 y 64 bits.
# Usa la librería bcm2835 para acceso a GPIO, SPI, I2C.
#=============================================================================

# ============================================================================
# Configuración de versión
# ============================================================================
VERSION := $(shell cat VERSION)

# ============================================================================
# Detección de arquitectura
# ============================================================================
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M), aarch64)
    ARCH := arm64
    ARCH_FLAGS := -march=armv8-a
else ifeq ($(UNAME_M), armv7l)
    ARCH := arm32
    ARCH_FLAGS := -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=hard
else ifeq ($(UNAME_M), armv6l)
    ARCH := arm32
    ARCH_FLAGS := -march=armv6zk -mfpu=vfp -mfloat-abi=hard
else
    ARCH := unknown
    ARCH_FLAGS :=
endif

# ============================================================================
# Compilador y flags
# ============================================================================
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2
CXXFLAGS += $(ARCH_FLAGS)
CXXFLAGS += -DVERSION=\"$(VERSION)\"
CXXFLAGS += -Iinclude -Iinclude/oled

# Detectar si estamos usando el stub de bcm2835
BCM2835_STUB := $(shell test -f include/bcm2835.h && grep -q "STUB" include/bcm2835.h && echo 1 || echo 0)

# Flags de enlazado
ifeq ($(BCM2835_STUB), 1)
    # Usando stub - no enlazar con bcm2835 real
    LDFLAGS := -lpthread
    CXXFLAGS += -DBCM2835_STUB
else
    # bcm2835 real
    LDFLAGS := -lbcm2835 -lpthread
endif

# ============================================================================
# Directorios
# ============================================================================
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# ============================================================================
# Archivos fuente
# ============================================================================
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/oled/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/engine/*.cpp)

# ============================================================================
# Archivos objeto (manteniendo jerarquía)
# ============================================================================
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/src/%.o, $(SOURCES))

# ============================================================================
# Target principal
# ============================================================================
TARGET := $(BIN_DIR)/App

# ============================================================================
# Reglas
# ============================================================================

.PHONY: all clean distclean install info

## Compilar todo
all: info $(TARGET)
	@echo ""
	@echo "=========================================="
	@echo "  Compilación completada exitosamente"
	@echo "  Binario: $(TARGET)"
	@echo "  Versión: $(VERSION)"
	@echo "  Arquitectura: $(ARCH)"
	@echo "=========================================="

## Mostrar información de compilación
info:
	@echo "Compilando para arquitectura: $(ARCH)"
	@echo "Versión: $(VERSION)"
	@echo "Compilador: $(CXX)"

## Enlazar el binario final
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@echo "Enlazando: $@"
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

## Crear directorio de objetos
$(OBJ_DIR)/src:
	mkdir -p $(OBJ_DIR)/src/oled
	mkdir -p $(OBJ_DIR)/src/engine

## Compilar archivos .cpp a .o
$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)/src
	@mkdir -p $(dir $@)
	@echo "Compilando: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

## Crear directorio de binarios
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

## Limpiar archivos objeto
clean:
	@echo "Limpiando archivos objeto..."
	rm -rf $(OBJ_DIR)
	@echo "Limpieza completada."

## Limpiar todo (incluyendo binario)
distclean: clean
	@echo "Limpiando binario..."
	rm -rf $(BIN_DIR)
	@echo "Limpieza completada."

## Instalar en el sistema (requiere sudo)
install: $(TARGET)
	@echo "Instalando $(TARGET) en /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/cnc-controller
	@echo "Instalación completada. Ejecute 'cnc-controller' para iniciar."

## Desinstalar
uninstall:
	@echo "Desinstalando..."
	sudo rm -f /usr/local/bin/cnc-controller
	@echo "Desinstalación completada."

# ============================================================================
# Dependencias automáticas
# ============================================================================
-include $(OBJECTS:.o=.d)

# Regla para generar archivos de dependencia
$(OBJ_DIR)/src/%.d: $(SRC_DIR)/%.cpp | $(OBJ_DIR)/src
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MM -MT $(@:.d=.o) $< > $@
