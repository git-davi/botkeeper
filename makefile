
CC = gcc
MAIN = main
LIB = -lptask 
LIBS = `allegro-config --cflags --libs`
TAR = extract
CFLAGS = -L. $(LIB) -lrt -pthread -isystemheaders -Wall -O2

program :
	$(CC) $(MAIN).c -o $(MAIN) $(LIBS) $(CFLAGS)

clean :
	rm -rf $(MAIN)

