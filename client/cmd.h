#ifndef CMD_H
#define CMD_H

int analy_cmd(char *cmd);// analysis command
void cmd_reg(int sockfd, vpChat temp);// registered command
void cmd_help();// help command
void cmd_log(int sockfd, vpChat temp);

#endif