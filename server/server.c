#include "cmd_parse.h"
#include "data.h"
#include "mySQL.h"
#include "mysocket.h"
int main() {
    time_t timep;
    time(&timep);
    char *timedata;  // get time data
    sqlite3 *db = NULL;
    char *errmsg;
    open_db(&db);  // open database
    creat_user_db(db, &errmsg);
    creat_online_db(db, &errmsg);
    int lfd;
    int cfd;
    int sfd;
    int rdy;
    stChat temp;
    struct sockaddr_in sin;
    struct sockaddr_in cin;
    int client[FD_SETSIZE]; //array of client socket
    int maxi;
    int maxfd; //max connect number
    fd_set rset;
    fd_set allset;
    socklen_t addr_len; // len of address
    int i;
    int n;
    int len;
    int opt = 1; // socket option
    char addr_p[20];
    mybzero(&sin);
    lfd = mysocket();
	// set default socket option
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    mybind(lfd, &sin);  // bind serer_addr to sockfd
    mylisten(lfd);      // listen on port, wait for client reply
    time(&timep);
    timedata = ctime(&timep);  // get time string
    timedata[strlen(timedata) - 1] = '\0';
    printf("%s:server start to wait for client to connect\n", timedata);

    maxfd = lfd; // initial max file descriptor
    maxi = -1;
	// initial client connect descriptor
    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }

    FD_ZERO(&allset);     // empty file descriptor set
    FD_SET(lfd, &allset);
	// server start to listen
    while (1) {
        rset = allset;

		// get present readable file descriptor number
        rdy = select(maxfd + 1, &rset, NULL, NULL, NULL);
        // process new connection request
        if (FD_ISSET(lfd, &rset)) {
            addr_len = sizeof(sin);
            cfd = myaccept(lfd, &cin, &addr_len);
            for (i = 0; i < FD_SETSIZE; i++) {     
                // printf("%d\t",client[i]);
                if (client[i] <= 0) {
                    client[i] = cfd;
                    break;
                }
            }

            // too many clients
            if (i == FD_SETSIZE) {
                printf("too many clients");
                exit(1);
            }

            FD_SET(cfd, &allset);
            if (cfd > maxfd) {
                maxfd = cfd;
            }

            if (i > maxi) {
                maxi = i;
            }

            if (--rdy <= 0) {
                continue;
            }
        }

        // process each connection descriptor
        for (i = 0; i < FD_SETSIZE; i++) {
            if ((sfd = client[i]) < 0) {
                continue;
            }

            if (FD_ISSET(sfd, &rset)) {
                printf("client sfd = %d connect successfully\n", sfd);
                n = read(sfd, &temp, sizeof(stChat));
                printf("client name = %s\n", temp.name);
                if (n == 0) {
                    printf(" client sfd = %d disconnect from server\n", sfd);
                    delete_online_db(db, &errmsg, sfd);
                    fflush(stdout);
                    close(sfd);
                    FD_CLR(sfd, &allset);
                    client[i] = -1;
                } else {
                    temp.sockfd = sfd;
                    // change client address to connect string
                    inet_ntop(AF_INET, &cin.sin_addr, addr_p, sizeof(addr_p));
                    addr_p[strlen(addr_p)] = '\0';
                    printf("client ip: %s, port num: %d\n", addr_p, ntohs(cin.sin_port));
                    int revert;
                    revert = cmd_user(db, &errmsg, &temp, sfd);
                    temp.revert = revert;
                    printf("start to send command to client\n");
                    if (revert < 5) {
                        printf("revert less than 5!\n");
                        mywrite(&temp);
                    } else if (revert == ALLOK) {
                        printf("send message\n");
                        write_online_all(db, &errmsg, &temp);
                    }
                    printf("send over\n");
                    memset(&temp, 0, sizeof(stChat));  // empty send message struct
                    if (n == 1) {
                        exit(1);
                    }
                }

                if (--rdy <= 0) {
                    break;
                }
            }
        }
    }

    close(lfd); // close socket
    return 0;
}
