APP=dwm_calendar
SRC=$(APP).c
objects=$(APP).o
#CFLAGS=-O3 -m64
#CFLAGS=-O0 -g3
GTK_CFLAGS=$(shell pkg-config gtk+-2.0 --cflags)
GTK_LDFLAGS=$(shell pkg-config gtk+-2.0 --libs)
all:$(APP)
$(APP): $(objects)
	gcc -o$(APP) $(objects) $(GTK_LDFLAGS)
$(APP).o: $(SRC)
	gcc -c -o$(APP).o $(CFLAGS) $(GTK_CFLAGS) $(SRC)
clean:
	rm -f $(APP) $(objects)
