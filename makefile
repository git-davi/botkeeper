
CC = gcc
OUT = game
TEST = test
FILES = main.c task.c util.c
TEST_FILES = main_prova.c util.c
LIBS = `allegro-config --cflags --libs` -lptask -lrt -pthread -lm
CFLAGS = -L. -isystemheaders -Imyheader -Wall -O2

$(OUT) :
	$(CC) $(FILES) -o $(OUT) $(LIBS) $(CFLAGS)

$(TEST) :
	$(CC) $(TEST_FILES) -o $(TEST) $(LIBS) $(CFLAGS)

clean :
	rm $(TEST) $(OUT) 
