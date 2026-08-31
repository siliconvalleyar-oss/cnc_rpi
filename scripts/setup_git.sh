#!/bin/bash
# ============================================================================
# setup_git.sh - Configuración del repositorio Git para CNC Controller
#
# Este script pregunta interactivamente los datos necesarios para crear
# el repositorio Git y subirlo a GitHub/GitLab.
#
# Uso: ./scripts/setup_git.sh
# ============================================================================

set -e

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo "=========================================="
echo "  Configuración del repositorio Git"
echo "  CNC Controller for Raspberry Pi"
echo "=========================================="
echo ""

# ============================================================================
# Recopilar información del usuario
# ============================================================================

read -p "Nombre de usuario de GitHub/GitLab: " GIT_USER
read -p "Nombre del repositorio [cnc_rpi]: " REPO_NAME
REPO_NAME=${REPO_NAME:-cnc_rpi}

echo ""
echo "Visibilidad del repositorio:"
echo "  1) Público"
echo "  2) Privado"
read -p "Seleccione [1]: " VISIBILITY_CHOICE
VISIBILITY_CHOICE=${VISIBILITY_CHOICE:-1}

if [ "$VISIBILITY_CHOICE" = "2" ]; then
    VISIBILITY="private"
    GH_VISIBILITY="--private"
else
    VISIBILITY="public"
    GH_VISIBILITY="--public"
fi

echo ""
read -p "Descripción del repositorio [CNC Controller for Raspberry Pi]: " REPO_DESC
REPO_DESC=${REPO_DESC:-"CNC Controller for Raspberry Pi - G-code interpreter with 5 Pololu drivers"}

# ============================================================================
# Verificar herramientas
# ============================================================================
echo ""
echo "Verificando herramientas..."

if ! command -v git &> /dev/null; then
    echo -e "${RED}Git no está instalado. Instálelo primero.${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Git instalado${NC}"

# Verificar si gh está disponible
GH_AVAILABLE=false
if command -v gh &> /dev/null; then
    GH_AVAILABLE=true
    echo -e "${GREEN}✓ GitHub CLI instalado${NC}"
else
    echo -e "${YELLOW}! GitHub CLI no disponible${NC}"
fi

# ============================================================================
# Configurar Git local
# ============================================================================
echo ""
echo "Configurando Git local..."

# Configurar usuario si no está configurado
CURRENT_NAME=$(git config --global user.name 2>/dev/null || echo "")
CURRENT_EMAIL=$(git config --global user.email 2>/dev/null || echo "")

if [ -z "$CURRENT_NAME" ]; then
    git config --global user.name "$GIT_USER"
    echo "Nombre configurado: $GIT_USER"
fi

if [ -z "$CURRENT_EMAIL" ]; then
    read -p "Email para Git: " GIT_EMAIL
    git config --global user.email "$GIT_EMAIL"
    echo "Email configurado: $GIT_EMAIL"
fi

# ============================================================================
# Inicializar repositorio
# ============================================================================
echo ""
echo "Inicializando repositorio Git..."

if [ -d .git ]; then
    echo -e "${YELLOW}Ya existe un repositorio Git en este directorio.${NC}"
    read -p "¿Desea reiniciar? (s/N): " RESET
    if [ "$RESET" = "s" ] || [ "$RESET" = "S" ]; then
        rm -rf .git
        git init
        echo "Repositorio reiniciado."
    fi
else
    git init
    echo "Repositorio inicializado."
fi

# Rama principal
git branch -M main 2>/dev/null || true

# ============================================================================
# Crear .gitignore
# ============================================================================
if [ ! -f .gitignore ]; then
    cat > .gitignore << 'EOF'
# Archivos objeto
obj/
*.o
*.d

# Binarios
bin/

# Archivos de compilación
*.a
*.so

# Editor
.vscode/
.idea/
*.swp
*.swo
*~

# OS
.DS_Store
Thumbs.db

# Logs
*.log
EOF
    echo -e "${GREEN}.gitignore creado${NC}"
fi

# ============================================================================
# Primer commit
# ============================================================================
echo ""
echo "Creando commit inicial..."

git add .
git commit -m "Initial commit: CNC Controller for Raspberry Pi

- G-code interpreter for 5-axis CNC
- Pololu stepper driver support (A4988/DRV8825)
- OLED SSD1306 display support
- bcm2835 library integration
- Compatible with ARM32 and ARM64"

echo -e "${GREEN}Commit inicial creado.${NC}"

# ============================================================================
# Crear repositorio remoto
# ============================================================================
echo ""
echo "Creando repositorio remoto..."

if [ "$GH_AVAILABLE" = true ]; then
    # Verificar autenticación
    if gh auth status &>/dev/null; then
        echo "Creando repositorio con GitHub CLI..."
        gh repo create "$REPO_NAME" \
            $GH_VISIBILITY \
            --description "$REPO_DESC" \
            --source=. \
            --remote=origin \
            --push
        echo -e "${GREEN}Repositorio creado y código subido.${NC}"
    else
        echo -e "${YELLOW}No autenticado en GitHub CLI.${NC}"
        echo "Ejecute 'gh auth login' y vuelva a intentar."
        echo ""
        echo "O use estos comandos manualmente:"
        echo "  git remote add origin https://github.com/$GIT_USER/$REPO_NAME.git"
        echo "  git push -u origin main"
    fi
else
    # Sin GitHub CLI - crear local y dar instrucciones
    echo -e "${YELLOW}GitHub CLI no disponible.${NC}"
    echo ""
    echo "El repositorio local está listo. Para subir a GitHub:"
    echo ""
    echo "1. Cree el repositorio en https://github.com/new"
    echo "   Nombre: $REPO_NAME"
    echo "   Visibilidad: $VISIBILITY"
    echo "   NO añada README, .gitignore ni licencia"
    echo ""
    echo "2. Ejecute estos comandos:"
    echo "   git remote add origin https://github.com/$GIT_USER/$REPO_NAME.git"
    echo "   git push -u origin main"
    echo ""

    # Preguntar si desea añadir remote
    read -p "¿Desea añadir el remote ahora? (s/N): " ADD_REMOTE
    if [ "$ADD_REMOTE" = "s" ] || [ "$ADD_REMOTE" = "S" ]; then
        git remote add origin "https://github.com/$GIT_USER/$REPO_NAME.git"
        echo "Remote añadido: https://github.com/$GIT_USER/$REPO_NAME.git"
        echo ""
        echo "Para subir el código:"
        echo "  git push -u origin main"
    fi
fi

# ============================================================================
# Resumen
# ============================================================================
echo ""
echo "=========================================="
echo "  Configuración completada"
echo "=========================================="
echo ""
echo "Repositorio: $REPO_NAME"
echo "Usuario: $GIT_USER"
echo "Visibilidad: $VISIBILITY"
echo ""
echo -e "${GREEN}¡Listo para desarrollar!${NC}"
echo ""
