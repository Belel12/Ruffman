INCLUDEPATH = src/includes
GCCFLAGS = -Wall -Wextra -Werror -I$(INCLUDEPATH)

ruffman: bin/main.o bin/ruffman.o bin/operations.o bin/utils.o
	gcc -o $@ $^

bin/main.o: src/main.c
	gcc $(GCCFLAGS) -c $< -o $@

bin/ruffman.o: src/ruffman.c
	gcc $(GCCFLAGS) -c $< -o $@

bin/operations.o: src/operations.c
	gcc $(GCCFLAGS) -c $< -o $@

bin/utils.o: src/utils.c
	gcc $(GCCFLAGS) -c $< -o $@