#include "./clientHandler.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void *clientHandler(void *args) {
   // jestem na 100% pewien że nie zamykam połączenia
   C_HANDLER *clientSocket = (args);
	clientSocket->used=1;

   char buff[255];
   inet_ntop(AF_INET, &(clientSocket->addr.sin_addr), buff, sizeof(buff));
   printf("New Connection(%lu) form: %s\n", clientSocket->id, buff);
	sleep(1);
   close(clientSocket->socket);

	clientSocket->used=0;
   return NULL;
}
