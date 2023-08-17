#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// 定义递归函数，用于在给定目录下查找目标文件或目录
void find(char const *path, char const *target)
{
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;
	if((fd = open(path, 0)) < 0){
		fprintf(2, "find: cannot open %s\n", path);
		exit(1);
	}
	// 打开指定路径的文件
	if(fstat(fd, &st) < 0){
		fprintf(2, "find: cannot stat %s\n", path);
		exit(1);
	}

	// 获取文件信息
	switch(st.type){
		case T_FILE:
			fprintf(2, "Usage: find dir file\n");
			exit(1);
		case T_DIR:
			if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
				printf("find: path too long\n");
				break;
			}
			strcpy(buf, path);
			p = buf + strlen(buf);
			*p++ = '/';
			while(read(fd, &de, sizeof(de)) == sizeof(de)){
				if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
					continue;
				memmove(p, de.name, DIRSIZ);
				p[DIRSIZ] = 0;
				if(stat(buf, &st) < 0){
        			printf("find: cannot stat %s\n", buf);
        			continue;
      			}
      			if(st.type == T_DIR){
      				find(buf, target);// 递归进入子目录
      			}else if (st.type == T_FILE){
      				if (strcmp(de.name, target) == 0)
      				{
      					printf("%s\n", buf); // 输出匹配的文件路径
      				}
      			}
			}
			break;
	}
	close(fd);
}

int
main(int argc, char *argv[]){
    if (argc != 3)
	{
		fprintf(2, "Usage: find dir file\n");
		exit(1);
	}
	char const *path = argv[1];
	char const *target = argv[2];
	find(path, target);// 调用递归查找函数
	exit(0);
}