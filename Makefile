# Nome do executável
TARGET = arkanoid

# Arquivos fonte
SRC = main.c

# Flags de compilação
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
# Flags para usar a Raylib instalada (local: /usr/local/lib)
RAYLIB_FLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt

# Regra principal: compilar
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(RAYLIB_FLAGS)

# Regra para limpar os arquivos gerados
clean:
	rm -f $(TARGET)