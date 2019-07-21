
CC = gcc
OUT = game
FILES = main.c task.c util.c
TEST_FILES = main_prova.c util.c
LIBS = `allegro-config --cflags --libs` -lptask -lrt -pthread -lm
CFLAGS = -L. -isystemheaders -Imyheader -Wall -O2

game :
	$(CC) $(FILES) -o $(OUT) $(LIBS) $(CFLAGS)

test :
	$(CC) $(TEST_FILES) -o "test" $(LIBS) $(CFLAGS)
