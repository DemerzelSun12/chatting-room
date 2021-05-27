#ifndef __DATA__
#define __DATA__
#define MAXSIZE 100
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sqlite3.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<termios.h>
#include<assert.h>
#define portnumber 8000 // port number
struct chat
{
	char name[20];	// username
	char passwd[20];// password
	int cmd;		// command
	int revert;		// return value
    char msg[800];	// send message
	int sockfd;
	char time[40];
};
typedef struct chat stChat;
typedef struct chat* vpChat;
enum cmd
{
	REG = 1,
	LOG = 2,
	ALL = 4,
	EXIT = 7,
	HELP = 8,
	ERROR = -1,
};
enum revert
{
	USERIN = 1,
	USEROUT = -1,
	PASSWDOK = 2,
	PASSWDNO = -2,
	ONLINEIN = 3,
	ONLINEOUT = -3,
	REGNO = -4,
	REGOK = 4,
	ALLOK = 6,
};
#endif
