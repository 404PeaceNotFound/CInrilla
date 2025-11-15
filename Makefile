CC = gcc
CFLAGS = -Wall -std=c99
LIBS = -lraylib -lm -lpthread -ldl -lrt -lGL -lX11
SRC = src/main.c src/game.c src/menu.c src/audio.c src/graphics.c src/creditos.c
TARGET = jogo

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run