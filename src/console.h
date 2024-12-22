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
 *	Handles user terminal input
 *
 *	consoleStruct: CONSOLE pointer casted to void pointer
 *
 *	returns: NULL on exit
 */
extern void *console(void *consoleStruct);

/**
 * Enum: LOG_TYPE
 *	--------------
 *
 *	Used exclusively by logger function
 *
 * I think that value names are self explanatory
 *
 * so there is no point in explaining them
 */

typedef enum {
   ERROR = 0,
   INFO = 1,
   SEVERE = 2,
} LOG_TYPE;

/**
 * Function: logger
 * ---------------
 *
 *	Displays and logs message
 *
 *	msg: string that will be logged/displayed
 *
 *	type: type of logged/displayed message
 *
 *	display: if 0 message will not be displayed in terminal
 *
 */
extern void logger(const char *msg, const LOG_TYPE type, const char display);

/**
 * Function setLogFile
 * -------------------
 *
 *	opens/cloases log file
 *
 *	fileName: name of desired log file
 *
 *	if set to NULL log file will be closed
 *
 */
extern void setLogFile(const char *fileName);
#endif
