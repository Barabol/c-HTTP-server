#ifndef __CLIENT_HANDLER__
#define __CLIENT_HANDLER__
#include <netinet/in.h>

/**
 * Structure: C_HANDLER
 * --------------------
 *
 *used by clientHandler function
 *
 *socket: [int] client connection
 *
 *addr: [sockaddr_in] client connection
 *
 *used: [unsigned char] parameter used for identifying usage of connection
 *
 *1(or other than 0): used
 *
 *0: free
 *
 *id: [unsigned long] passed just because
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
 *handles singular connection
 *
 *handleStruct: C_HANDLER pointer casted to void pointer
 *
 *returns: NULL on exit
 */
void *clientHandler(void *handlerStruct);
#endif
