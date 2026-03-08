CC = gcc

SRC = src/main.c libs/glad.c

CFLAGS = -Iinclude

LIBS = -lglfw -lGL -lm

OUT = fuck

all:
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(OUT)

run:
	./$(OUT)

clean:
	rm -f $(OUT)
