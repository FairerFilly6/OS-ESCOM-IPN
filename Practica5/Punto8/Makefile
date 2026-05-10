CC      = gcc
CFLAGS  = -Wall -Wextra

PROCS   = proc_suma proc_resta proc_multiplicacion proc_traspuesta proc_inversa proc_lector

all: $(PROCS) main

proc_suma: proc_suma.c matrices.h
	$(CC) $(CFLAGS) -o proc_suma proc_suma.c

proc_resta: proc_resta.c matrices.h
	$(CC) $(CFLAGS) -o proc_resta proc_resta.c

proc_multiplicacion: proc_multiplicacion.c matrices.h
	$(CC) $(CFLAGS) -o proc_multiplicacion proc_multiplicacion.c

proc_traspuesta: proc_traspuesta.c matrices.h
	$(CC) $(CFLAGS) -o proc_traspuesta proc_traspuesta.c

proc_inversa: proc_inversa.c matrices.h
	$(CC) $(CFLAGS) -o proc_inversa proc_inversa.c

proc_lector: proc_lector.c
	$(CC) $(CFLAGS) -o proc_lector proc_lector.c

main: main.c matrices.h
	$(CC) $(CFLAGS) -o main main.c

run: all
	./main

clean:
	rm -f $(PROCS) main *.txt