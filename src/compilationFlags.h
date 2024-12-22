#ifndef __COMPILATION_FLAGS__
#define __COMPILATION_FLAGS__

//-----------------------------------------------//

/**
 * Broadcast port
 *
 * DEFAULT: 80
 */
#define PORT 80

/**
 * Max concurrent connections
 *
 * DEFAULT: 10 low i know
 */
#define MAX_CONNECTIONS 10

/**
 * Maximal amount of elements in console buffer
 *
 * DEFAULT: 1024
 */
#define MAX_CONSOLE_BUFFER_LENGTH 1024

/**
 * Size of buffer used for receiving data form client
 *
 * DEFAULT: 1024
 */
#define REC_BUFFER_SIZE 1024

/**
 * Length of string representing path
 * eg:
 * - /index.html ~ 12
 * - /site/index.html ~ 17
 * 40 should do the trick
 *
 * DEFAULT: 40
 */
#define MAX_ACCESS_POINT_SIZE 40

//-----------------------------------------------//

/**
 * Name and location of log file
 *
 * DEFAULT: "./log.txt"
 */
#define DEFAULT_ERROR_FILE "./log.txt"

/**
 * Name of default file given to client
 *
 * DEFAULT: "index.html"
 */
#define DEFAULT_FILE_NAME "index.html"

/**
 * Accept only this type of request
 *
 * DEFAULT: "GET"
 */
#define ACCEPT_ONLY "GET"

/**
 * Accept only this type of file to be accessed
 *
 * DEFAULT: "html"
 */
#define ALLOWED_EXTENSION "html" 

//-----------------------------------------------//

/**
 * If this is defined log files will use same
 * colors as those printed in console.
 *
 * If not logs will be without collors
 *
 * less command does not support /033[nm colors
 * but more command does
 */
#define COLOR_IN_LOG_FILES

/**
 * If this option is defined
 * there will be shown full HTTP request
 * in console
 * (it will not be logged)
 */
#define SHOW_REQUEST

/**
 * If this option is defined
 * server socket will set flags
 * for it to be able to quickly
 * be reused
 */
#define REUSE_ADDR_AND_PORT

/**
* If this option is defined
* client will be able to
* access hidden files
* eg:
*
* - .config
* - .hiddendir/index.html
*/
//#define ALLOW_HIDDEN_FILES

/**
* if this option is defined 
* client will be able to use
* ".." as path
* eg:
*
* ../etc
*/
//#define ALLOW_DOUBLE_DOT_OPERATOR

//-----------------------------------------------//

#endif
