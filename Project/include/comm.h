#ifndef _COMM_H
#define _COMM_H

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include "./json.h"
#include <sqlite3.h>
#include <termios.h>    /*PPSIX ÷’∂Àøÿ÷∆∂®“Â*/

#define LOG (printf("%s (%d) - <%s>\n",__FILE__,__LINE__,__FUNCTION__),printf)
#define LINLOG (printf("%d- <%s>\n",__LINE__,__FUNCTION__),printf)

typedef struct FILEFD
{
    int connect_fd;
    int uart_fd;
}m_filefd;


#include "uart.h"
#include "bejson.h"
#include "sql.h"
#include "function.h"




#endif
