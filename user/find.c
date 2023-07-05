#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void find(char* path,char* filename)
{
    char buff[512],buff1[512],*p,*q;
    int fd;
    struct dirent de;
    struct stat st;
    char* name;
    char new_filename[DIRSIZ];
    strcpy(new_filename,filename);
    memset(new_filename+strlen(filename), ' ', DIRSIZ-strlen(filename));
    if((fd = open(path,0)) < 0) {
        fprintf(2,"find: cannot open %s\n",path);
        return; 
    }
    if(fstat(fd,&st) < 0)
    {
        fprintf(2,"find: cannot stat %s\n",path);
        close(fd);
        return; 
    }
    switch(st.type) {
        case T_FILE:
            //printf("%s %d %d %l dasdas\n", fmtname(path), st.type, st.ino, st.size);
            name = fmtname(buff);
            if(!strcmp(name,filename)) 
            {
                //write(1,name,strlen(name));
                write(1,path,strlen(path));
                write(1,"/",1);
                write(1,filename,strlen(filename));
                write(1,"\n",1);
                //printf("%s/%s\n",path,name);
            }
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buff){
            printf("ls: path too long\n");
            break;
            }
            strcpy(buff, path);
            p = buff+strlen(buff);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buff, &st) < 0){
                    printf("ls: cannot stat %s\n", buff);
                    continue;
                }
                name = fmtname(buff);
                
                //printf("%s# %s# %d\n",new_filename,name,strcmp(name,new_filename));
                if(st.type == T_DIR && name[0]!='.')
                {
                    strcpy(buff1, path);
                    q = buff1+strlen(buff1);
                    *q++ = '/';
                    for(;*name!=' ';name++,q++)
                    {
                        *q = *name;
                    }
                    *q = '\0';
                    //printf("%s %s\n",buff1,new_filename);
                    find(buff1,filename);
                }
                else if(!strcmp(name,new_filename)) 
                {
                    
                    write(1,path,strlen(path));
                    write(1,"/",1);
                    write(1,filename,strlen(filename));
                    write(1,"\n",1);
                    //printf("%s/%s\n",path,name);
                    
                }
                //printf("%s %d %d %d\n", fmtname(buff), st.type, st.ino, st.size);
            }
            break;
    }
}

int main(int argc,char* argv[])
{
    if(argc < 3)
    {
        printf("need path or filename\n");
        exit(1);
    }    
    find(argv[1],argv[2]);
    exit(0);
    return 0;
}