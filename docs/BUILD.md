# Compilación del Proyecto

## Requisitos

- g++ con soporte C++17
- make
- bcm2835

## Comandos de Compilación

### Compilar todo
```bash
make
```

### Limpiar archivos objeto
```bash
make clean
```

### Limpiar todo (incluye binario)
```bash
make distclean
```

### Mostrar información de compilación
```bash
make info
```

### Instalar en el sistema
```bash
sudo make install
```

## Estructura de Compilación

El Makefile detecta automáticamente la arquitectura:

| Arquitectura | Flag |
|--------------|------|
| armv6l (RPi 1/Zero) | -march=armv6zk -mfpu=vfp -mfloat-abi=hard |
| armv7l (RPi 2/3) | -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=hard |
| aarch64 (RPi 3/4/5) | -march=armv8-a |

## Archivos Generados

```
obj/
├── src/
│   ├── main.o
│   ├── cnc_controller.o
│   └── oled/
│       ├── SSD1306_OLED.o
│       ├── SSD1306_OLED_graphics.o
│       ├── SSD1306_OLED_Print.o
│       └── SSD1306_OLED_font.o
```

```
bin/
└── App
```

## Versión en Compilación

La versión se define en el archivo `VERSION` y se pasa como macro:

```makefile
VERSION := $(shell cat VERSION)
CXXFLAGS += -DVERSION=\"$(VERSION)\"
```

En el código:
```cpp
std::cout << "Version: " << VERSION << std::endl;
```

## Solución de Problemas

### Error: "bcm2835.h: No such file"
```bash
sudo ./scripts/install_deps.sh
```

### Error: "undefined reference to bcm2835_*"
Verificar que bcm2835 está instalada:
```bash
ls /usr/local/lib/libbcm2835.a
```

### Error de permisos en GPIO
```bash
sudo ./bin/App
```
