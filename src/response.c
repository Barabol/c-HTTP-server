#include "./clientHandler.h"
#include "./console.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
void buildResponse(const char *accessPoint, C_HANDLER *clientSocket) {
   static const char *res = "HTTP/1.1 200 OK\n"
                            "ContentType: image/x-icon\n\n";
   char *buffer = NULL;
   FILE *resFile = fopen(accessPoint, "r");
   if (!resFile) {
      logger("Requested file not found", ERROR, 1);
      sendError(clientSocket);
      return;
   }

   fseek(resFile, 0, SEEK_END);
   const unsigned long fileLen = ftell(resFile);
   fseek(resFile, 0, SEEK_SET);
   const unsigned long headerLen = strlen(res);

   buffer = malloc(sizeof(char) * (fileLen + headerLen + 1));

   if (!buffer) {
      logger("Was not able to allocate memory", SEVERE, 1);
      sendError(clientSocket);
      fclose(resFile);
      return;
   }

   memcpy(buffer, res, headerLen);
   buffer[headerLen] = '\n';

   unsigned long reedFromFile = headerLen;
   while ((buffer[reedFromFile] = fgetc(resFile)) != EOF) {
      reedFromFile++;
   }
   buffer[reedFromFile] = 0;

   send(clientSocket->socket, buffer, reedFromFile, 0);

   fclose(resFile);
   free(buffer);
}
void sendError(C_HANDLER *clientSocket) {
   // ewentualne dodanie int resErrCode i char* resErrMsg
   // miało by to sens ale narazie nie moge nawet wysyłać plików )-:
   static const char *res = "HTTP/1.1 404 Not Found\n"
                            "ContentType: text/plain\n"
                            "\n404 Not Found";
   long resLen = strlen(res);
   logger("Connection denied", INFO, 1);
   send(clientSocket->socket, res, resLen, 0);
}
