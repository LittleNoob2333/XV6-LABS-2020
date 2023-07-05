#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

int main(int argc,char* argv[])
{
    int p[2];
    int ret = pipe(p);
    int status;
    if(ret == -1)
    {
        printf("pipe error\n");
        exit(1);
    }
    int pid = fork();
    char buff[6];
    char* s = "received";
    if(pid == 0)
    {
        read(p[0],buff,sizeof buff);
        write(p[1],"pong",4);
        close(p[1]);
        printf("%d: %s %s\n",getpid(),s,buff);
    }
    else{
        write(p[1],"ping",5);
        wait(&status);
        read(p[0],buff,sizeof buff);
        close(p[0]);
        printf("%d: %s %s\n",getpid(),s,buff);
        
    }
    exit(0);
    return 0;
}
