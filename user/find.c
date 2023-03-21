#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path)
{
	static char buf[DIRSIZ+1];
	char *p;

	for(p = path + strlen(path); p >= path && *p != '/'; p--)
		;
	p++;
	if(strlen(p) >= DIRSIZ)
		return p;
	memmove(buf, p, sizeof(p));
	memset(buf+strlen(p), 0, DIRSIZ - strlen(p));
	return buf;
}

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
		find(buf, name);
		if(!strcmp(de.name,name))
			fprintf(1, "%s\n", buf);
	}
	
	/*
	switch(st.type){
		case T_FILE:
			if(!strcmp(fmtname(path), name)){
				fprintf(1, "%s\n", path);
				break;
			}
		case T_DIR:
			if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
				fprintf(2, "find: path too long\n");
				break;
			}
			strcpy(buf, path);
			p = buf + strlen(buf);
			*p++ = '/';
			while(read(fd, &de, sizeof(de))== sizeof(de)){
				if(de.inum == 0)
					continue;
				memmove(p, de.name, DIRSIZ);
				char *end = buf+strlen(buf);
				*end = '\x00';
				find(buf, name);
			}
			break;

	}*/
	close(fd);
}

int main(int argc, char *argv[])
{
	if(argc < 3){
		fprintf(2, "Please use:\n\t\tfind dir filename\n");
		exit(1);
	}
	find(argv[1], argv[2]);

	exit(0);
}
