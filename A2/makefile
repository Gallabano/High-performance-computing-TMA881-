
openMP : openMP.c
	gcc openMP.c -o openMP.o -O2 -lm -Wall -march=native

openMP2: openMP2.c
	gcc openMP2.c -o openMP2.o -O2 -lm -Wall -march=native

valgrind: openMP2.c
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./openMP2.o

clean : #Deletes all .o files
	@rm *.o