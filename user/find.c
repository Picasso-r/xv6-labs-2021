#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *name)
{
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;
	if((fd = open(path, 0)) < 0){
		fprintf(2, "find: cannot open %s\n", path);
		exit(1);
	}
	
	if(fstat(fd, &st) < 0){
		fprintf(2, "find: cannot open stat %s\n", path);
		exit(1);
	}
//	assert(st.type == T_DIR);
	if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
		fprintf(2, "find: path too long\n");
	strcpy(buf, path);
	p = buf + strlen(buf);
	*p++ = '/';
	while(read(fd, &de, sizeof(de))== sizeof(de)){
		if(de.inum == 0)
			continue;
		memmove(p, de.name, DIRSIZ);
		char *end = buf+strlen(buf);
		*end = '\x00';
		//fprintf(1, "%s\n", buf);
		if(stat(buf, &st) < 0){
			fprintf(2, "find: cannot stat %s\n", buf);
			continue;
		}
		if(st.type == T_DIR && (strcmp(de.name, ".") == 0||strcmp(de.name, "..") == 0))
			continue;
		if(!strcmp(de.name,name))
			fprintf(1, "%s\n", buf);
		st.type==T_DIR?find(buf, name):1;
	}
	
	close(fd);
}

int main(int argc, char *argv[])
{
	if(argc < 3){
		fprintf(2, "Lack of arguments! Please use like:\n\tfind dir filename\n");
		exit(1);
	}
	find(argv[1], argv[2]);

	exit(0);
}
