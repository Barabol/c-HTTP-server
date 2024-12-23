#include "./clientHandler.h"
#include "./compilationFlags.h"
#include "console.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
/**
 * Function: getAccessPoint
 * ------------------------
 *
 * req: pointer to full request
 *
 * ap: pointer to access point (it will save int there)
 *
 * fileIncluded: on soccess it will have bool value
 *
 * 1 if there is file included
 *
 * 0 if not
 *
 * len: int pointer on success it will eq length of accessPoint
 *
 *
 * returns:
 *
 * 0 if successfully found access point
 *
 * 1 if there wan no access point defined
 */
int getAccessPoint(char *req, char *ap, int *fileIncluded, int *len) {
   unsigned int point = 0;
   char requestType[10]; // wiem w 7 moge się zmieścić (wliczając \0)

	ap[0]='.';
   for (int z = 0; req[point] != ' '; point++, z++)
      requestType[z] = req[point];
   // printf("Request type: %s\n", requestType);
#ifdef ACCEPT_ONLY
   for (int x = 0; x < 10; x++) {
      if (requestType[x] == 0)
         break;
      if (requestType[x] != ACCEPT_ONLY[x])
         return 1;
   }
#endif
   // ten if jest dosyć nie potrzebny bo wiadomo, że tam spacja będzia ale
   // zostawie
   if (req[point] == ' ')
      point++;

   int lastFileMention = -1;
   int lastDirMention = -1;

   int fileMentioned = 0;
   int z;
   char lastChar = 0;
   for (z = 1; req[point] != ' ' && req[point] != '\n' && req[point] != 0;
        point++, z++) {

      if (req[point] == '\\')
         req[point] = '/';

      if (req[point] == '/')
         lastDirMention = z;

#ifndef ALLOW_DOUBLE_DOT_OPERATOR
      if (req[point] == '.' && lastChar == '.')
         return 2;
#endif

#ifndef ALLOW_HIDDEN_FILES
      if (req[point] == '.' && (lastChar == 0 || lastChar == '/'))
         return 3;
#endif

      if (req[point] == '.' && lastChar != '.')
         lastFileMention = z;
      ap[z] = req[point];

      lastChar = req[point];
   }
   if (lastFileMention < lastDirMention)
      lastFileMention = -1;
   *fileIncluded = lastFileMention > -1 ? 1 : 0;
   // printf("LFM: %d,%c\n", *fileIncluded,ap[lastFileMention]);
#ifdef ALLOWED_EXTENSION
   lastFileMention++;
   for (int x = 0; lastFileMention <= z && lastFileMention != 0;
        lastFileMention++, x++) {
      if (ALLOWED_EXTENSION[x] != ap[lastFileMention])
         return 4;
   }
#endif
   printf("Access point: %s, Len %d, ap: %c\n", ap, z, ap[z]);
   *len = z;
   if (lastChar != '/') {
      ap[z] = '/';
      ap[z + 1] = 0;
      (*len)++;
   }
   return 0;
}
void *clientHandler(void *args) {
   C_HANDLER *clientSocket = (args);
   clientSocket->used = 1;

   // "czemu dałem by było statyczne?" pewnie o tym myślisz
   // zgadza się. Sam nie wiem
   static char msg[70];
   static char ip[20];

   char buff[REC_BUFFER_SIZE];
   ssize_t bufferSize = recv(clientSocket->socket, buff, REC_BUFFER_SIZE, 0);
   char accessPoint[MAX_ACCESS_POINT_SIZE];

   if (bufferSize == 0) {
      close(clientSocket->socket);
      sprintf(msg, "Thread #%lu received faulty connection", clientSocket->id);
      clientSocket->used = 0;
      logger(msg, ERROR, 1);
      return NULL;
   }

   inet_ntop(AF_INET, &(clientSocket->addr.sin_addr), ip, sizeof(ip));
   sprintf(msg, "New Connection(%lu) form: %s", clientSocket->id, ip);
   logger(msg, INFO, 1);

#ifdef SHOW_REQUEST
   for (int x = 0; x < bufferSize; x++)
      printf("%c", buff[x]);
   puts(" ");
#endif

   // dać to na switcha by wypisywało inne wiadomości względem tego co zwraca
   // getAccessPoint
   int accessPointLen = -1;
   int isFileIncluded = 0;
   int holder =
       getAccessPoint(buff, accessPoint, &isFileIncluded, &accessPointLen);
   // printf("holder: %d\n", holder);
   if (holder) {
      close(clientSocket->socket);
      switch (holder) {
      case 2:
         sprintf(msg,
                 "Thread #%lu received illegal \"..\" path operation from %s",
                 clientSocket->id, ip);
         break;
      case 3:
         sprintf(msg, "Thread #%lu received illegal hidden file path from %s",
                 clientSocket->id, ip);
         break;
      case 4:
         sprintf(msg, "Thread #%lu received illegal file type from %s",
                 clientSocket->id, ip);
         break;
      default:
         sprintf(msg, "Thread #%lu received faulty access point from %s",
                 clientSocket->id, ip);
         break;
      }

      clientSocket->used = 0;
      logger(msg, SECURITY, 1);
      return NULL;
   }

#ifdef DEFAULT_FILE_NAME
   if (!isFileIncluded) {
      int fileLen = 0;
      for (fileLen = 0; DEFAULT_FILE_NAME[fileLen] != 0; fileLen++)
         ;

      if (fileLen != 0) {
         fileLen++;
         if (MAX_ACCESS_POINT_SIZE - 1 < accessPointLen + fileLen) {
            close(clientSocket->socket);
            sprintf(msg, "Thread #%lu received illegal path length from %s",
                    clientSocket->id, ip);
            clientSocket->used = 0;
            logger(msg, SECURITY, 1);
            return NULL;
         }
         for (int x = 0; x < fileLen; x++)
            accessPoint[accessPointLen + x] = DEFAULT_FILE_NAME[x];
         accessPoint[accessPointLen + fileLen] = 0;
			printf("AP: %s\n",accessPoint);
      } else {
         close(clientSocket->socket);
         clientSocket->used = 0;
         logger("Default file was not provided", ERROR, 1);
         return NULL;
      }
   }
#endif

   sleep(1);
   close(clientSocket->socket);

   clientSocket->used = 0;
   return NULL;
}
