#include "./console.h"
#include "./colors.h"
#include <stdio.h>
#include <sys/socket.h>
void *console(void *consoleStruct) {
   char buff[255];
   // obowiązkowe GOTO
	CONSOLE* cStruct = consoleStruct;
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
