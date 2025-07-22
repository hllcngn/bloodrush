all:
	gcc main.c \
	    dungen-nethacklike.c \
    	    dungen-mansion.c	-I. -lncurses -o bloodrush
