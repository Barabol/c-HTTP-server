#include "./src/clientHandler.h"
#include "./src/console.h"
#include "src/compilationFlags.h"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv) {
   // scope by setFile był zwolniony
   // potem to przeniose do osobnych funkcnji bo już się syf robi w mainie
   {
      char setFile = 0;
      for (int x = 1; x < argc; x++) {
         if (argv[x][0] == '-' && argv[x][1] == 'l') {
            if ((x + 1) < argc && argv[x + 1][0] != ' ') {
               setLogFile(argv[x + 1]);
               setFile = 1;
            } else {
               setLogFile(DEFAULT_ERROR_FILE);
               setFile = 1;
               logger("There was no file name provided, default file name will "
                      "be chosen",
                      ERROR, 1);
            }
         }
      }
      if (!setFile)
         setLogFile(DEFAULT_ERROR_FILE);
   }
   unsigned char status;
   int serverSocket;
   struct sockaddr_in address;

   serverSocket = socket(AF_INET, SOCK_STREAM, 0);

   if (serverSocket < 0) {
      logger("Cannot start socket", SEVERE, 1);
      setLogFile(NULL);
      return 1;
   }

   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(PORT);

#ifdef REUSE_ADDR_AND_PORT
   int optVal = 1;
   if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR,
                  &optVal, sizeof(optVal))) {
      logger("Cannot set socket options", SEVERE, 1);
      setLogFile(NULL);
      return 3;
   }
#endif
   if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
      logger("Cannot bind socket", SEVERE, 1);
      setLogFile(NULL);
      return 4;
   }

   if (listen(serverSocket, MAX_CONNECTIONS) < 0) {
      logger("Cannot start listener", SEVERE, 1);
      setLogFile(NULL);
      return 5;
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

   logger("Server started", INFO, 1);
   while (status) {
      clientSocket =
          accept(serverSocket, (struct sockaddr *)&clientAddress, &cliSocLen);

      // na wszelki wypadek chyba nie jest potrzebne ale może się przydać
      if (!status)
         break;

      if (clientSocket < 0) {
         logger("Cannot establish connection to client", ERROR, 1);
         continue;
      }
      { // kolejny scope czemu nie?
         int connection;
         for (int connection = 0; connection < MAX_CONNECTIONS; connection++) {
            if (cliHandler[connection].used == 0) {
               cliHandler[connection].addr = clientAddress;
               cliHandler[connection].socket = clientSocket;
               cliHandler[connection].id = connection;
               pthread_create(&thread[connection], NULL, clientHandler,
                              (void *)&(cliHandler[connection]));
               connection = -1;
               break;
            }
         }
         // chyba redundantne?
         if (connection == -1) {
            logger("All threads are used, dropping connection", ERROR, 1);
            close(clientSocket);
         }
      }
   }

   logger("Stopping server", INFO, 1);
   close(serverSocket);
   for (int x = 0; x < MAX_CONNECTIONS; x++) {
      if (cliHandler[x].used) {
         pthread_detach(thread[x]);
      }
   }
   // na wszelki wypadek
   pthread_join(consoleThread, 0);
   setLogFile(NULL);
   return 0;
}
