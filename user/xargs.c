#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

// 读取一行输入，将其分割为参数并返回参数数量
int readline(char *new_argv[32], int curr_argc){
	char buf[1024];
	int n = 0;
	while(read(0, buf+n, 1)){
		if (n == 1023)
		{
			fprintf(2, "argument is too long\n");
			exit(1);
		}
		if (buf[n] == '\n')
		{
			break;
		}
		n++;
	}
	buf[n] = 0;
	if (n == 0)return 0;
	int offset = 0;
	// 分割参数并存储到 new_argv 数组
	while(offset < n){
		new_argv[curr_argc++] = buf + offset;
		while(buf[offset] != ' ' && offset < n){
			offset++;
		}
		while(buf[offset] == ' ' && offset < n){
			buf[offset++] = 0;
		}
	}
	return curr_argc;
}

int main(int argc, char const *argv[])
{
	if (argc <= 1)
	{
		fprintf(2, "Usage: xargs command (arg ...)\n");
		exit(1);
	}
	char *command = malloc(strlen(argv[1]) + 1);
	char *new_argv[MAXARG];
	strcpy(command, argv[1]);
	// 复制参数到 new_argv 数组
	for (int i = 1; i < argc; ++i)
	{
		new_argv[i - 1] = malloc(strlen(argv[i]) + 1);
		strcpy(new_argv[i - 1], argv[i]);
	}

	int curr_argc;
	// 读取输入行，执行命令并等待子进程结束
	while((curr_argc = readline(new_argv, argc - 1)) != 0)
	{
		new_argv[curr_argc] = 0;
		if(fork() == 0){
			exec(command, new_argv);
			fprintf(2, "exec failed\n");
			exit(1);
		}
		wait(0);
	}
	exit(0);
}