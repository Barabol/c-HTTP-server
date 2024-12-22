#include "./src/clientHandler.h"
#include "./src/console.h"
#include "src/compilationFlags.h"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main(int argc, char **argv) {
   unsigned char status;
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

   if (listen(serverSocket, MAX_CONNECTIONS) < 0) {
      return 4;
   }

   pthread_t consoleThread;
   CONSOLE cStruct;
   cStruct.status = &status;
   cStruct.serverSocket = serverSocket;
   pthread_create(&consoleThread, NULL, console, &cStruct);

   pthread_t thread[MAX_CONNECTIONS];
   int clientSocket;
   socklen_t cliSocLen = sizeof(clientSocket);
   struct sockaddr_in clientAddress;

   status = 1;

   C_HANDLER cliHandler[MAX_CONNECTIONS];

   for (int x = 0; x < MAX_CONNECTIONS; x++)
      cliHandler[x].used = 0;

   while (status) {
      clientSocket =
          accept(serverSocket, (struct sockaddr *)&clientAddress, &cliSocLen);

      // na wszelki wypadek chyba nie jest potrzebne ale może się przydać
      if (!status)
         break;

      if (clientSocket < 0) {
         puts("he?");
         continue;
      }

      for (int x = 0; x < MAX_CONNECTIONS; x++) {
         if (cliHandler[x].used == 0) {
            cliHandler[x].addr = clientAddress;
            cliHandler[x].socket = clientSocket;
            cliHandler[x].id = x;
            pthread_create(&thread[x], NULL, clientHandler,
                           (void *)&(cliHandler[x]));
            break;
         }
      }
   }

   for (int x = 0; x < MAX_CONNECTIONS; x++) {
      if (cliHandler[x].used) {
         printf("Killing Thread: %d\n", x);
         pthread_detach(thread[x]);
      }
   }

   pthread_join(consoleThread, 0);
}
