#include "data.h"
void mybzero(struct sockaddr_in *sin);
int mysocket();
void mybind(int lfd,struct sockaddr_in *sin);
void mylisten(int lfd);
int myaccept(int lfd,struct sockaddr_in *cin,socklen_t * addr_len);
void mywrite(vpChat temp);
