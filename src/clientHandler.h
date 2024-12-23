#ifndef __CLIENT_HANDLER__
#define __CLIENT_HANDLER__
#include <netinet/in.h>

/**
 * Structure: C_HANDLER
 * --------------------
 *
 *	Used by clientHandler function
 *
 *	socket: [int] client connection
 *
 *	addr: [sockaddr_in] client connection
 *
 *	used: [unsigned char] parameter used for identifying usage of connection
 *
 *	1(or other than 0): used
 *
 *	0: free
 *
 *	id: [unsigned long] passed just because
 *
 */
typedef struct {
   int socket;
   struct sockaddr_in addr;
   unsigned char used;
   unsigned long id;
} C_HANDLER;

/**
 * Function: clientHandler
 * -----------------------
 *
 *	Handles singular connection
 *
 *	handleStruct: C_HANDLER pointer casted to void pointer
 *
 *	returns: NULL on exit
 */
void *clientHandler(void *handlerStruct);

/**
 * Function: buildResponse
 * -----------------------
 *
 * Builds and sends response to client
 *
 * accessPoint: file path and file name
 *
 * clientSocket: C_HANDLER pointer as pointer
 */
void buildResponse(const char *accessPoint, C_HANDLER *clientSocket);

/**
 * Function: sendError
 * -------------------
 *
 * Sends 404 not found
 *
 * clientSocket: C_HANDLER pointer as pointer
 */
void sendError(C_HANDLER *clientSocket);
#endif
