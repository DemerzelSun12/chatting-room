#include "data.h"

/**
 * assignment struct server_addr_in
 **/
void mybzero(struct sockaddr_in *sin) {
    bzero(sin, sizeof(struct sockaddr_in));
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr =
        htonl(INADDR_ANY);  // Convert ip address to network byte order
    sin->sin_port =
        htons(portnumber);  // Convert port number to network byte order
}

/**
 * Call the socket function to create a TCP protocol socket
 **/
int mysocket() {
    int lfd;
    if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) ==
        -1)  // AF_INET:IPV4;SOCK_STREAM:TCP
    {
        fprintf(stderr, "Socket error:%s\n\a", strerror(errno));
        exit(1);
    }
    return lfd;
}

/**
 * Call the bind function to bind the serer_addr structure to sockfd
 **/
void mybind(int lfd, struct sockaddr_in *sin) {
    if (bind(lfd, (struct sockaddr *)(sin), sizeof(struct sockaddr)) == -1) {
        fprintf(stderr, "Bind error:%s\n\a", strerror(errno));
        exit(1);
    }
}

/**
 * start to listen to port, wait for client request
 **/
void mylisten(int lfd) {
    if (listen(lfd, 20) == -1) {
        fprintf(stderr, "Listen error:%s\n\a", strerror(errno));
        exit(1);
    }
}

/**
 * Accept client's request
 **/
int myaccept(int lfd, struct sockaddr_in *cin, socklen_t *addr_len) {
    int cfd;
    if ((cfd = accept(lfd, (struct sockaddr *)cin, addr_len)) == -1) {
        fprintf(stderr, "Accept error:%s\n\a", strerror(errno));
        exit(1);
    }
    return cfd;
}
void mywrite(vpChat temp) {
    int num;
    if ((num = write(temp->sockfd, temp, sizeof(stChat))) == -1) {
        printf("socket number is %d!\n", temp->sockfd);
        printf("send error!\n");
    }
}
