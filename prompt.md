INSTRUCCIONES COMPLETAS PARA EL ASISTENTE (PROMPT ÚNICO Y DEFINITIVO)
========================================================================

Debes generar un proyecto C++ completo para Raspberry Pi (compatible con 32 y 64 bits) que use la librería bcm2835, siguiendo la estructura de carpetas y archivos que se detalla abajo. El proyecto debe ser autocontenido, compilable con make, y debe incluir todo el código fuente, cabeceras, scripts de instalación, documentación y archivos de configuración.
ordenar el codigo existente , es el codigo de una raspberry y un CNC con 5 pololu interpreta el codigo G y lo corre en la raspberry
 
ESTRUCTURA OBLIGATORIA (crear todos los directorios y archivos)
----------------------------------------------------------------
├── bin
│   └── cncApp                           # binario final
├── config
│   ├── config.cfg                    # vacío o con ejemplos
│   └── hardware.cfg                  # vacío
├── docs
│   ├── ACTIVITY.md
│   ├── API.md
│   ├── ARCHITECTURE.md
│   ├── ARQUITECTURA.md
│   ├── BLUETOOTH.md
│   ├── BUILD.md
│   ├── CHANGELOG.md
│   ├── CONTRIBUTING.md
│   ├── DEPLOY.md
│   ├── DESING.md                     # (mantén este nombre aunque sea "DESIGN")
│   ├── DIAGRAMS.md
│   ├── doxygen/                      # directorio (vacío)
│   ├── HARDWARE.md
│   ├── INSTALL.md
│   ├── LEARNINGS.md                  # DEBES LEER Y COMPLETAR (si existe)
│   ├── MEMORY_MAP.md
│   ├── PROMPT.md
│   ├── REPORT.md
│   ├── ROADMAP.md
│   ├── RULES.md
│   ├── SECURITY.md
│   ├── SETUP.md
│   ├── SKILLS.md
│   ├── TESTING.md
│   ├── TODO.md
│   ├── TROUBLESHOOTING.md
│   ├── USAGE.md
│   └── WORKFLOW.md                   # DEBES LEER Y COMPLETAR (si existe)
├── examples                          # directorio vacío
├── generate_basic_src.sh             # script auxiliar (puede estar vacío)
├── include
│   ├── core/                         # cabeceras del núcleo (vacío)
│   ├── drivers/                      # cabeceras de drivers (vacío)
│   ├── engine/                       # cabeceras del motor (vacío)
│   ├── cnc.hpp                  # cabecera del magnetómetro
│   ├── libraries/                    # cabeceras externas (vacío)
│   ├── nlohmann/
│   │   └── json.hpp                  # librería JSON (puede ser vacía o la oficial)
│   ├── oled/
│   │   ├── SSD1306_OLED_font.hpp
│   │   ├── SSD1306_OLED_graphics.hpp
│   │   ├── SSD1306_OLED.hpp
│   │   └── SSD1306_OLED_Print.hpp
│   └── security/                     # cabeceras de seguridad (vacío)
├── LICENSE                           # licencia (ej. MIT)
├── Makefile                          # archivo de compilación (debe definir VERSION)
├── obj/                              # directorio para objetos (se creará durante la compilación)
├── README.md                         # DEBES COMPLETARLO
├── scripts/
│   └── install_deps.sh               # script para instalar dependencias
├── src/
│   ├── engine/                       # fuentes del motor (vacío)
│   ├── main.cpp                      # archivo principal (ver formato abajo)
│   └── oled/
│       ├── SSD1306_OLED.cpp
│       ├── SSD1306_OLED_font.cpp
│       ├── SSD1306_OLED_graphics.cpp
│       └── SSD1306_OLED_Print.cpp
└── VERSION                           # archivo con el número de versión (ej. 0.1.0)


REQUISITOS FUNCIONALES DEL CÓDIGO
----------------------------------
- Usa la librería bcm2835 para el acceso a GPIO, SPI, I2C, etc.
- Incluye siempre #include <memory> para gestión inteligente de memoria.
- Define un namespace, por ejemplo "Device", y dentro una clase "Device_t".
- El main.cpp DEBE tener el siguiente formato exacto (sin cambios en la lógica):

    #include <memory>
    #include "cnc.hpp"   // o la ruta adecuada

    int main() {
        auto device = std::make_unique<Device::Device_t>();
        device->run();
        return 0;
    }

- La clase Device_t debe tener un método run() que contenga la lógica principal (puede ser un bucle simple, encender un LED, leer un sensor, etc.) y que demuestre el uso de bcm2835.
- No uses new/delete explícitos; la memoria se libera automáticamente al salir del main gracias al unique_ptr.

VERSIÓN DE LA APLICACIÓN (EN TIEMPO DE COMPILACIÓN)
---------------------------------------------------
- La versión de la aplicación NO se lee del archivo VERSION en tiempo de ejecución, sino que se define como una macro en tiempo de compilación.
- El Makefile debe pasar la versión al compilador usando -DVERSION="$(VERSION)" (o similar), leyendo el número del archivo VERSION.
- En el código (por ejemplo, en Device_t o en main), se debe mostrar la versión al iniciar la aplicación (por ejemplo, imprimiendo por consola: "App v1.2.3").
- Además, se debe soportar un argumento de línea de comandos (por ejemplo, --version) que muestre la versión y termine la ejecución.
- Todo el código debe estar completamente documentado con comentarios (explicando qué hace cada función, clase, y partes importantes).

CREACIÓN DEL REPOSITORIO GIT (DESDE SHELL CONSOLA)
---------------------------------------------------
El proceso de generación del proyecto debe incluir la creación de un repositorio Git. Para ello, el script o el asistente debe interactuar con el usuario y preguntar:

    - Nombre de usuario en la plataforma (GitHub, GitLab, etc.).
    - Credenciales (token de acceso o pedir que ya estén configuradas globalmente).
    - Nombre del repositorio (que coincidirá con el nombre del proyecto).
    - Visibilidad: público o privado.

Con estos datos, el sistema debe ejecutar los siguientes comandos desde la shell (por ejemplo, al final del script de generación):

    1. git init
    2. git add .
    3. git commit -m "Initial commit"
    4. gh repo create <nombre> --public (o --private) --source=. --remote=origin --push
       (si se usa GitHub CLI) o alternativamente:
       - Crear el repositorio vía API (curl) y luego añadir el remote.
    5. git push -u origin main (o master)

Si no se dispone de GitHub CLI, se debe proporcionar instrucciones claras para que el usuario cree el repositorio manualmente y luego ejecute los comandos git remote add y git push. El asistente debe generar un script (por ejemplo, setup_git.sh) que automatice todo el proceso y que pueda ejecutarse después de la generación de archivos.

El script debe verificar si gh está instalado; si no, debe ofrecer la opción de crear el repositorio localmente y mostrar los pasos para el remoto.

SCRIPTS Y MAKEFILE
------------------
- scripts/install_deps.sh: Debe instalar bcm2835 (descargando y compilando desde el sitio oficial o usando apt) y cualquier otra dependencia (g++, make, git, etc.). Debe funcionar en Raspberry Pi de 32 y 64 bits.
- Makefile:
  * Debe leer la versión del archivo VERSION (ej. con $(shell cat VERSION)) y pasarla como -DVERSION="..." a todos los objetos.
  * Compila todos los .cpp de src/ y sus subdirectorios, generando los .o en obj/ manteniendo la jerarquía (ej. obj/src/main.o, obj/src/oled/SSD1306_OLED.o).
  * El binario final se llama "App" y se coloca en bin/.
  * Enlaza con la librería bcm2835 (opciones -lbcm2835).
  * Debe soportar ambas arquitecturas (32 y 64 bits) mediante flags condicionales o detección automática.
  * Incluye objetivos: all, clean, distclean, install (opcional).
- generate_basic_src.sh: puede ser un script auxiliar para generar archivos fuente (opcional, puede estar vacío).
- setup_git.sh: script que pregunta interactivamente los datos y configura el repositorio Git.

DOCUMENTACIÓN (archivos .md)
----------------------------
- Lee (si existen) los archivos docs/LEARNINGS.md y docs/WORKFLOW.md y complétalos con información relevante al proyecto (aprendizajes, flujo de trabajo, etc.). Si no existen, créalos con contenido útil.
- Completa TODOS los archivos .md de docs/ con descripciones coherentes y apropiadas para cada tema (API, arquitectura, instalación, uso, etc.). No dejes ninguno vacío.
- README.md debe incluir:
  * Nombre del proyecto y descripción general.
  * Requisitos (Raspberry Pi, bcm2835, Git, gh opcional).
  * Instrucciones de compilación e instalación (usando make y el script de dependencias).
  * Indicación explícita de que la aplicación es compatible con Raspberry Pi de 32 bits y 64 bits.
  * Un ejemplo básico de uso.
  * Mención de que la versión se muestra al inicio y con --version.
  * Instrucciones para configurar el repositorio Git (ejecutar setup_git.sh o los pasos manuales).

CONTENIDO ADICIONAL
-------------------
- Los archivos de cabecera y fuente de la pantalla OLED (SSD1306) deben estar implementados, aunque sea con funciones básicas o declaraciones, pero que compilen sin errores.
- HMC5883L.hpp debe declarar una clase para el magnetómetro (puede ser un esqueleto con métodos vacíos).
- El archivo VERSION debe contener un número de versión (ej. 0.1.0).

COMENTARIOS Y DOCUMENTACIÓN DEL CÓDIGO
--------------------------------------
- Todo el código fuente (archivos .cpp y .hpp) debe estar ampliamente comentado en español o inglés (consistente), explicando:
  * El propósito de cada clase, método y función.
  * Los parámetros y retornos.
  * La lógica importante.
  * Las decisiones de diseño.
- También se deben incluir comentarios de tipo Doxygen (si se desea) para facilitar la generación de documentación.
- El código debe ser legible y seguir buenas prácticas de programación (nombres descriptivos, const-correctness, etc.).

ENTREGABLE FINAL
----------------
El asistente debe generar todos los archivos y carpetas con el contenido adecuado. El código debe compilar sin errores en una Raspberry Pi (simulado o real) usando make. La documentación debe estar completa y bien redactada. Todo debe ser funcional, coherente y seguir las especificaciones dadas, incluyendo la configuración del repositorio Git mediante interacción con el usuario.

NOTA: Este prompt contiene todas las instrucciones previas fusionadas, incluyendo la gestión de versión en tiempo de compilación, la documentación exhaustiva del código y la creación del repositorio Git con preguntas interactivas. Ejecuta cada paso y genera el proyecto completo.
