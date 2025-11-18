# ==============================================================================
# Variáveis de Configuração
# ==============================================================================

# Compilador C
CC = cc

# Diretório principal dos arquivos-fonte
SRC_DIR = src

# NOME do executável final
TARGET = CInrilla

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c99 -g
# Flags de otimização (comente a linha acima e descomente esta para produção)
# CFLAGS = -Wall -Wextra -std=c99 -O2

# Diretórios que contêm arquivos .c que precisam ser compilados
SUBDIRS = \
    $(SRC_DIR)/core \
    $(SRC_DIR)/scenes \
    $(SRC_DIR)/systems \
    $(SRC_DIR)/entities \
    $(SRC_DIR)/ui \
    $(SRC_DIR)/utils

# Flags e bibliotecas Raylib (ajuste conforme sua instalação)
RAYLIB_FLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt

# ==============================================================================
# Geração Automática de Arquivos
# ==============================================================================

# Encontra todos os arquivos .c no diretório src e subdiretórios
C_FILES = $(shell find $(SRC_DIR) -name "*.c")

# Gera a lista de arquivos objeto (.o) correspondentes
O_FILES = $(patsubst $(SRC_DIR)/%.c, obj/%.o, $(C_FILES))

# ==============================================================================
# Regras de Build
# ==============================================================================

# Regra principal: Cria o executável
.PHONY: all
all: $(TARGET)

$(TARGET): $(O_FILES)
	@echo "🔗 Linking executable $@"
	$(CC) $(O_FILES) -o $@ $(RAYLIB_FLAGS)

# Regra para compilar cada arquivo .c em um arquivo objeto .o
obj/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D) # Cria o diretório obj/subdir se não existir
	@echo "Compiling $< -> $@"
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# Regra para compilar o arquivo main.c
obj/main.o: $(SRC_DIR)/main.c
	@mkdir -p obj
	@echo "Compiling $< -> $@"
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# ==============================================================================
# Regras de Limpeza
# ==============================================================================

.PHONY: clean
clean:
	@echo "🗑️ Removing build files..."
	rm -f $(TARGET)
	rm -rf obj