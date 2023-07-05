#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/param.h"
int c = 0;

char** get_allinput(int argc,char* input,char* argv[])
{
    char** lines = (char**)malloc(MAXARG*sizeof(char*));
    int len = strlen(input);
    char buf[128];
    int pos=0;

    for(int i = 0; i < len; i++)
    {
        if(input[i]!= '\n')
        {
            buf[pos++] = input[i];
            //printf("%c\n",input[i]);
        } 
        else{
            lines[c] = (char*)malloc(128*sizeof(char));
            strcpy(lines[c],buf);
            //printf("%s#\n",lines[c]);
            c++;
            pos=0;
            memset(buf,0,128);
        }
    }
    lines[c] = 0;
    return lines;
}

//首要问题: 前面的命令是什么？

int main(int argc,char* argv[])
{
    //一行
    int status;
    /*readinput*/
    char buf[128];
    read(0, buf, sizeof(buf));
    char** lines = get_allinput(argc,buf,argv);
    
    char** arg = (char**)malloc(MAXARG*sizeof(char*));
    for(int i = 1; i < argc; i++)
    {
        arg[i-1] = (char*)malloc(128*sizeof(char));
        strcpy(arg[i-1],argv[i]);
    }
    //printf("%s\n",arg[0]);
    arg[argc-1] = (char*)malloc(128*sizeof(char));
    for(int i = 0; i < c; i++)
    {
        int pid = fork();
        if(pid == 0)
        {
            //printf("%s\n",lines[0]);
            strcpy(arg[argc-1],lines[i]);
            arg[argc] = 0;
            exec(argv[1],arg);
            exit(0);
        }
        else {
            wait(&status);
        }
    }
    exit(0);
    return 0;
}