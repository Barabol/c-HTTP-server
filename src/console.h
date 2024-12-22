#ifndef __CONSOLE__
#define __CONSOLE__

/**
 * Structure: CONSOLE
 * ------------------
 *
 * status: [unsigned char pointer] pointer to variable
 *
 * if set to 0 it will stop main loop used to create new connections
 *
 *	serverSocket: [int] server socket descriptor passed to allow for
 *connection termination
 *
 */
typedef struct {
   unsigned char *status;
   int serverSocket;
} CONSOLE;

/**
 * Function: console
 * -----------------
 *
 *handles user terminal input
 *
 *consoleStruct: CONSOLE pointer casted to void pointer
 *
 *returns: NULL on exit
 */
extern void *console(void *consoleStruct);
#endif
