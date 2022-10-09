CFLAGS = -O0 -flto -lpthread -lm
ARGS = -t1 -l1000 1
DEPS = computenewton.h writenewton.h #Header files
OBJ = computenewton.o writenewton.o #Object files

.PHONY: all #Without it, implicit rules will try to build the executable "all", since the prereqs are ".o" files.
all: newton

%.o: %.c $(DEPS)
	gcc -c -o $@ $< $(CFLAGS) 

newton: $(OBJ) newton.o
	gcc -o $@ $^ $(CFLAGS) -g

valgrind: newton #Don't forget to put -g if you want to debug
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./newton $(ARGS)

clean:
	@rm *.o newton *.ppm