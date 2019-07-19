
CC = gcc
OUT = game
FILES = main.c task.c util.c
LIBS = `allegro-config --cflags --libs` -lptask -lrt -pthread 
CFLAGS = -L. -isystemheaders -Imyheader -Wall -O2

program :
	$(CC) $(FILES) -o $(OUT) $(LIBS) $(CFLAGS)


