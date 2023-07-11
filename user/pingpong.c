#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
    int p[2];
    if(pipe(p) == -1)//fail to create a pipe
        exit(1);
    if(fork()==0){//child
        char msg[1], msg2[1];
        read(p[0],msg,1);
        close(p[0]);
        fprintf(0, "%d: received ping\n", (int)getpid());
        write(p[1], msg2, 1);
        close(p[1]);
    }
    else{//parent
        char msg[1], msg1[1];
        write(p[1], msg1, 1);
        close(p[1]);
        wait(0);
        read(p[0],msg,1);
        fprintf(0, "%d: received pong\n", (int)getpid());
        close(p[0]);
    }
    exit(0);
}