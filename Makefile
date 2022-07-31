bigint_test: bin/test.o bin/bigint.o
	cc -o bin/bigint_test bin/test.o bin/bigint.o

bin/test.o: src/test.c src/bigint.h
	mkdir -p bin
	cc -c src/test.c -o bin/test.o 

bin/bigint.o: src/bigint.c src/bigint.h
	mkdir -p bin
	cc -c src/bigint.c -o bin/bigint.o 