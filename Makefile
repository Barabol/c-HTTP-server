build:
	gcc main.c ./src/*.h ./src/*.c -o srv.out
debug:
	gcc main.c ./src/*.h ./src/*.c -p -o srv.out
run:
	sudo ./srv
