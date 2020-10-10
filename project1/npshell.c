#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

struct cmd{
    char cmd[32];
};

int main()
{
    setenv("PATH", "bin:.", 1);
    char buf[255];
    while(1)
    {
        int cmdCnt=0;
        struct cmd *cmdset;
        cmdset = malloc(sizeof(struct cmd)*10);
        printf("$>");
        fgets(buf,255,stdin);
        if(strcmp(buf,"exit\n")==0)
            break;
        for(int i=0,j=0;i<strlen(buf)-1;i++,j++)
        {
            if(buf[i]=='|'){
                cmdCnt++;
                j=0;
                continue;
            }
            cmdset[cmdCnt].cmd[j] = buf[i];
        }
        for(int i=0;i<=cmdCnt;i++){
            int pid = fork();
            if(pid==0)
                execlp(cmdset[i].cmd,cmdset[i].cmd,NULL);
            else {
		        int child_status;
		        wait(&child_status);
            }
        }
        free(cmdset);
            
    }
    return 0;
}