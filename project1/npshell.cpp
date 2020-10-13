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
#include <vector>
#include <queue>
#include <iostream>
using namespace std;
struct command{
    int argc;
    char* cmd[256];
};

int main()
{
    setenv("PATH", "bin:.", 1);
    char buf[255];
    while(1)
    {
        int cmdCnt=0;
        int child_status=0;
        const char delim[2] = "|";
        string str;
        char *token;
        queue<char*> cmdset;
        cout <<'%';
        fgets(buf,255,stdin);
        if(strcmp(buf,"exit\n")==0)
            break;
        buf[strlen(buf)-1]='\0';
        token = strtok(buf, delim);
        while( token != NULL ) 
        {
            cmdCnt++;
            if(token!=NULL)
                cmdset.push(token);
            token = strtok(NULL, delim);
            if(token==NULL)
                break;
        }

        for(int i=0;i<cmdCnt;i++){
            char *cmds;
            //strcpy(cmds,cmdset.front());
            strcpy(cmds,cmdset.front());
            int pid = fork();
            if(pid==0){
                execlp(cmds,cmds,NULL);
                child_status=1;
                break;
            }
            else {
		        wait(&pid);
            }
        }
        if(child_status==1)
            break;
    }
    return 0;
}