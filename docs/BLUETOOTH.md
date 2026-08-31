# Bluetooth

## Visión General

El controlador CNC puede ser extendido para soportar comunicación Bluetooth, permitiendo control inalámbrico desde un dispositivo móvil o computadora.

## Implementación Propuesta

### Hardware Adicional

No se requiere hardware adicional en Raspberry Pi 3/4/5 (Bluetooth integrado). Para Raspberry Pi Zero/2W, usar un módulo USB Bluetooth.

### Configuración BlueZ

```bash
sudo apt-get install bluez bluez-tools
```

### Perfil SPP (Serial Port Profile)

Para comunicación serial por Bluetooth:

```bash
# Editar configuración
sudo nano /etc/systemd/system/dbus-org.bluez.service

# Añadir --compat al ejecutable
ExecStart=/usr/lib/bluetooth/bluetoothd --compat
```

## Uso Propuesto

1. Emparejar dispositivo Bluetooth
2. Conectar al puerto serie virtual
3. Enviar comandos G-code como por serial

## Código de Ejemplo

```cpp
// bluetooth.hpp (propuesto)
class BluetoothServer {
public:
    bool initialize();
    void listen();
    std::string readCommand();
    void sendResponse(const std::string& response);
};
```

## Consideraciones de Seguridad

- Autenticación por PIN
- Lista blanca de dispositivos
- Cifrado de comunicación
