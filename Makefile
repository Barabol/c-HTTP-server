build:
	gcc main.c ./src/*.h ./src/*.c -o srv.out
debug:
	gcc main.c ./src/*.h ./src/*.c -p -o srv.out
	# sudo is used to allow usage of port 80
	sudo gdb ./srv.out
run:
	sudo ./srv.out
