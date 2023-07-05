#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

int main(int argc,char* argv[])
{
    int p[100][2];
    int pid[100];
    int status;
    int pos = 0;
    pipe(p[pos]);
    int is_forked[100];
    memset(is_forked,0,sizeof(is_forked));
    pid[pos] = fork();
    if(pid[pos] > 0)
    {
        close(p[pos][0]);
        for(int i = 2; i <= 35; i++)
        {
            write(p[pos][1],&i,sizeof(int));
        }
        close(p[pos][1]);
        wait(&status);
        //exit(0);
    }
    else if(pid[pos] == 0)
    {
        pos++;
        int i = 0;
        pipe(p[pos]);
        int n = 0;
        close(p[pos-1][1]);
        read(p[pos-1][0],&i,sizeof(int));
        printf("prime %d\n",i);
        while(read(p[pos-1][0],&n,sizeof(int)))
        {
            if(n%i)
            {
                if(!is_forked[pos]) pid[pos] = fork();
                if(pid[pos] > 0)
                {
                    is_forked[pos] = 1;
                    close(p[pos][0]);
                    write(p[pos][1],&n,sizeof(int));
                }
                else
                {
                    pos++;
                    close(p[pos-1][1]);
                    pipe(p[pos]);
                    read(p[pos-1][0],&i,sizeof(int));
                    printf("prime %d\n",i);
                }
            }
        }
        close(p[pos][1]);
        wait(&status);
        exit(0);
    }
    exit(0);
    return 0;
}