#include "./console.h"
#include "./colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

void *console(void *consoleStruct) {
   char buff[255];
   // obowiązkowe GOTO
   CONSOLE *cStruct = consoleStruct;
loop:
   scanf("%s", buff);
   printf(RED);
   printf(B_GREEN);
   printf(BLINK);
   printf(BOLD);
   printf("Res: %s\n", buff);
   printf(CLEAR);
   if (buff[0] == 'q') {
      *cStruct->status = 0;
      shutdown(cStruct->serverSocket, SHUT_RD);
      return NULL;
   }
   goto loop;
}
// prywatna funkcja co  robi wszystko z logowaniem
void logger_(const char *msg, const LOG_TYPE type, const char display,
             FILE *file) {
   // dodanie timestampów było by miłe...
   // [INFO|19:30|22.12.2024] Info
   // ^coś w tym stylu?^
   static FILE *errFile = NULL;
   static char TypeNames[][10] = {
       "ERROR",
       "INFO",
   };
   static char Colors[][10] = {
       RED,
       BLUE,
   };

   if (file != NULL) {
      errFile = file;
      return;
   }

   if (display)
      printf("%s%s[%s]:%s%s%s\n", Colors[type], BOLD, TypeNames[type], WHITE,
             msg, CLEAR);

   if (!errFile) {
      printf("%s%s[%s]:%sLog file isn't defined or cannot be opened%s\n",
             Colors[ERROR], BOLD, TypeNames[ERROR], WHITE, CLEAR);
      return;
   }
   // przydało by się w końcu dodac zapisywanie do tego pliku ale komu to
   // potrzebne
}
//----------------------implementacja------------------
void logger(const char *msg, const LOG_TYPE type, const char display) {
   logger_(msg, type, display, NULL);
}
void setLogFile(const char *fileName) {
   static FILE *file;
   if (fileName) {
      file = fopen(fileName, "w+");
      logger_("", ERROR, 0, file);
      logger("Started logging to file", INFO, 1);
   } else if (file) {
      logger("Stopped logging to file", INFO, 1);
      fclose(file);
   }
}
//------------------------------------------------------
