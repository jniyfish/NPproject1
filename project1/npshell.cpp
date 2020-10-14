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

struct command {
	int argc;
	char* argv[256];
};

int parse_cmd(char* buffer, struct command *ptr);

int main()
{
    setenv("PATH", "bin:.", 1);
    char buf[255];
    while(1)
    {
        int cmdCnt=0;
        int child_status=0;
        const char delim[3] = "|!";
        char *token;
        char* saveptr = NULL;
        queue<char*> cmdset;
        struct command commands[256];

        cout <<'%';
        fgets(buf,255,stdin);
        if(strcmp(buf,"exit\n")==0)
            break;
        buf[strlen(buf)-1]='\0';
        token = strtok_r(buf, delim,&saveptr);
        commands[0].argc = parse_cmd(token,&commands[0]);
        if(token == NULL){
            printf("Error\n");
        }
        int index = 1;
        while( token != NULL ) 
        {
            cmdCnt++;
            if(token!=NULL){
                cmdset.push(token);
                token = strtok_r(NULL, delim,&saveptr);
                commands[index].argc = parse_cmd(token,&commands[index]);
                index++;
            }
            commands[index].argc = 0;
            commands[index].argv[0] = strdup("\0");
            if(token==NULL)
                break;
        }
        
        for(int i=0;i<cmdCnt;i++){
            int pid = fork();
            if(pid==0){
                execvp(commands[i].argv[0],commands[i].argv);
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


int parse_cmd(char* buffer, struct command *ptr){
    if(buffer == NULL)
        return 0;
    char* substr = NULL;
	char* saveptr = NULL;
	char* dupstr = strdup(buffer);
    const char delim[2] = " ";

    substr = strtok(dupstr, delim);
	ptr->argv[0] = substr;

	if (substr == NULL) {
		printf("Error: parse_cmd() failed\n");
		return -1;
	}
	int index = 1;
	while (substr) {
		substr = strtok(NULL, delim);
		ptr->argv[index] = substr;
		index++;
	}
	return index - 1;
}