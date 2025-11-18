CC = gcc
CFLAGS = -Wall -std=c99
LIBS = -lraylib -lm -lpthread -ldl -lrt -lGL -lX11

# coloca aqui os files que vao rodar
SRC = src/main.c \
	  src/logica/game_logic.c \
	  src/telas/menu.c \
	  src/audio/audio.c \
	  src/telas/scene_manager.c \
	  src/telas/creditos.c
TARGET = jogo

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run