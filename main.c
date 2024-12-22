#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#define PORT 80
unsigned char status;
void *clientHandler(void *args) {
   // jestem na 100% pewien że nie zamykam połączenia
   int clientSocket = (int)(args);
   char buff[255];
   inet_ntop(AF_INET, &clientSocket, buff, sizeof(buff));
   printf("Connection form IP: %s\n", buff);
   return NULL;
}
#define RED "\033[31m"
#define CLEAR "\033[0m"
void *console() {
   char buff[255];
	// obowiązkowe GOTO 
loop:
   scanf("%s", buff);
   printf(RED);
   printf("Res: %s\n", buff);
   printf(CLEAR);
   if (buff[0] == 'q') {
      status = 0;
      return NULL;
   }
   goto loop;
}
int main(int argc, char **argv) {
   int serverSocket;
   struct sockaddr_in address;

   serverSocket = socket(AF_INET, SOCK_STREAM, 0);

   if (serverSocket < 0)
      return 1;

   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(PORT);

   if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
      return 3;
   }

   if (listen(serverSocket, 10) < 0) {
      return 4;
   }

   pthread_t consoleThread;
   pthread_create(&consoleThread, NULL, console, NULL);

   pthread_t thread;
   int clientSocket;
   socklen_t cliSocLen = sizeof(clientSocket);
   struct sockaddr_in clientAddress;

   status = 1;
   while (status) {
      clientSocket =
          accept(serverSocket, (struct sockaddr *)&clientAddress, &cliSocLen);
      if (clientSocket < 0) {
         puts("he?");
         continue;
      }
      // PRAWDOPODOBNIE będę musiał podmienić castowanie int -> void* na
      // wskaźnik do ciekawej strukturki by móc przekazać więcej ciekawych
      // itemków (-:
      pthread_create(&thread, NULL, clientHandler, (void *)clientSocket);

      //-----------------------------DO-WYWALENIA-POTEM----------------
      char buff[255]; // nwm czemu wypisuje źle na początku ip
      inet_ntop(AF_INET, &(clientAddress.sin_addr), buff, sizeof(buff));
      printf("Connection form IP: %s\n", buff);
      //---------------------------------------------------------------
   }
   pthread_join(consoleThread, 0);
}
