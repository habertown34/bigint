bigint: #bigint.c bigint.h main.c
	gcc -I./ -o bin/bigint src/bigint.c src/bigint.h src/main.c