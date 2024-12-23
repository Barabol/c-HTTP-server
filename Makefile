build:
	gcc main.c ./src/*.h ./src/*.c -o srv.out
debug:
	gcc main.c ./src/*.h ./src/*.c -p -o srv.out
	# sudo is used to allow usage of port 80
	# if sudo is used and log file does not exist 
	# log file will be created with elevated privlages
	sudo gdb ./srv.out
run:
	sudo ./srv.out
