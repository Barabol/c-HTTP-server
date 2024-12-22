build:
	gcc main.c ./src/*.h ./src/*.c -o srv

run:
	sudo ./srv
