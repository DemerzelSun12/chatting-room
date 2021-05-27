#include "cmd.h"

#include "data.h"
#include "my_function.h"
int analy_cmd(char *cmd)  // analysis command
{
    if (strcmp(cmd, "reg") == 0) {
        return REG;
    } else if (strcmp(cmd, "log") == 0) {
        return LOG;
    } else if (strcmp(cmd, "help") == 0) {
        return HELP;
    } else if (strcmp(cmd, "exit") == 0) {
        return EXIT;
    } else {
        return ERROR;
    }
}

void cmd_reg(int sockfd, vpChat temp) {
    char passwd[20];
    char passwd1[20];
    printf("Enter username:");
    scanf("%s", temp->name);
    printf("Enter password:");
    scanf("%s", passwd);
    printf("Confirm passsword:");
    scanf("%s", passwd1);
    if (strcmp(passwd, passwd1) == 0) {
        printf("Registering, please wait a moment\n");
        strcpy(temp->passwd, passwd);
        printf("Send registration information\n");
        mywrite(sockfd, temp);
        memset(temp, 0, sizeof(stChat));
        temp->revert = -1;
        read(sockfd, temp, sizeof(stChat));
        if (temp->revert == REGOK) {
            sleep(1);
            printf("Successfully registered, welcome to use!\n");
            printf("\n");
        } else if (temp->revert == REGNO) {
            sleep(1);
            printf("\n");
            printf("This username has already been registered for an account!\n");
        } else {
            sleep(1);
            printf("\n");
            printf("The server did not send you a confirmation message!\n");
        }
    } else {
        printf("\n");
        printf("The password you entered is inconsistent!\n");
    }
}
void cmd_help() {
    printf("**********Welcome to use this chatting room**********\n");
    printf("         1. registration enter reg         \n");
    printf("         2、log in enter log       \n");
    printf("         3、assistant information enter help         \n");
    printf("         4、give up login enter exit         \n");
}
void cmd_log(int sockfd, vpChat temp) {
    printf("Enter username:");
    scanf("%s", temp->name);
    printf("\nEnter password:");
    char passwd[20];
    scanf("%s", passwd);
    printf("\n");
    strcpy(temp->passwd, passwd);
    mywrite(sockfd, temp);
    printf("\n");
    printf("Signing in, please wait!\n");
}