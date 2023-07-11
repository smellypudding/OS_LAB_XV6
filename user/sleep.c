#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, int *argv[])
{
  if(argc < 2){
    fprintf(2, "Missing argument, Usage: sleep times\n");
    exit(1);
  }//need argument to specify the sleeping time
  else if (argc > 2){
    fprintf(2, "Too many arguments, Usage: sleep times\n");
    exit(1);
  }//only need one arg
  else{
    int time = atoi((const char*)(argv[1]));
    if(sleep(time)!=0)
        fprintf(2, "Error in sleep sys_call!\n");
  }
  exit(0);
}