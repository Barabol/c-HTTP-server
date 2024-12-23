#include "./console.h"
#include "./colors.h"
#include "compilationFlags.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>

void *console(void *consoleStruct) {
   char buff[255];
   // obowiązkowe GOTO
   CONSOLE *cStruct = consoleStruct;
loop:
   scanf("%254s", buff);
   // printf("Res: %s\n", buff);
   if (buff[0] == 'q') {
      *cStruct->status = 0;
      logger("Shutting down server (console input)", INFO, 1);
      shutdown(cStruct->serverSocket, SHUT_RD);
      return NULL;
   }
   if (*cStruct->status)
      goto loop;
   return NULL;
}
// prywatna funkcja co  robi wszystko z logowaniem
void formatTime(char *timeStr) {
   int x, z;
   for (x = 0; timeStr[x] != ' '; x++)
      ;
   x++;
   for (z = 0; timeStr[x] != '\n'; x++, z++)
      timeStr[z] = timeStr[x];
   timeStr[z] = 0;
}
void logger_(const char *msg, const LOG_TYPE type, const char display,
             FILE *file) {
   static FILE *errFile = NULL;
   static char TypeNames[][10] = {
       "ERROR",
       "INFO",
       "SEVERE",
       "SECURITY",
   };
   static char Colors[][10] = {
       RED,
       BLUE,
       RED,
       RED,
   };

   static time_t rawtime;
   static struct tm *timeinfo;
   time(&rawtime);
   timeinfo = localtime(&rawtime);

   if (file != NULL) {
      errFile = file;
      return;
   }
   char buff[MAX_CONSOLE_BUFFER_LENGTH];
   char *timeStr = asctime(timeinfo);
   formatTime(timeStr);
   if (display)
      printf("%s%s[%s|%s]: %s%s%s\n", Colors[type], BOLD, TypeNames[type],
             timeStr, WHITE, msg, CLEAR);

   if (!errFile) {
      printf("%s%s[%s|%s]: %sLog file isn't defined or cannot be opened%s\n",
             Colors[ERROR], BOLD, TypeNames[ERROR], timeStr, WHITE, CLEAR);
      return;
   }
   if (strlen(buff) > MAX_CONSOLE_BUFFER_LENGTH - 70) {
      printf("%s%s[%s|%s]: %sMAX_CONSOLE_BUFFER_LENGTH wasn't configured "
             "properly, preavoius message will not be saved to log file (this "
             "one too)%s\n",
             Colors[ERROR], BOLD, TypeNames[ERROR], timeStr, WHITE, CLEAR);
      return;
   }
   // sprintf(buff, "%s%s[%s|%s]:%s%s\n", Colors[type], BOLD, TypeNames[type],
   //       timeStr, WHITE, CLEAR);
   // printf(">>> %lu\n",strlen(buff));
#ifndef COLOR_IN_LOG_FILES
   sprintf(buff, "[%s|%s]: %s\n", TypeNames[type], timeStr, msg);
#endif
#ifdef COLOR_IN_LOG_FILES
   sprintf(buff, "%s%s[%s|%s]: %s%s%s\n", Colors[type], BOLD, TypeNames[type],
           timeStr, WHITE, msg, CLEAR);
#endif
   buff[MAX_CONSOLE_BUFFER_LENGTH - 1] = 0;

   fwrite(buff, sizeof(char), strlen(buff) / sizeof(char), errFile);
}
//----------------------implementacja------------------
void logger(const char *msg, const LOG_TYPE type, const char display) {
   logger_(msg, type, display, NULL);
}
void setLogFile(const char *fileName, int override) {
   static FILE *file;
   if (fileName) {
      char *option;
      if (override)
         option = "w+";
      else
         option = "a+";
      file = fopen(fileName, option);
      logger_("", ERROR, 0, file);
      logger("Started logging to file", INFO, 1);
   } else if (file) {
      logger("Stopped logging to file", INFO, 1);
      fclose(file);
   }
}
//------------------------------------------------------
