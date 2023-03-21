#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	char buf = 'a';
	int p2c[2], c2p[2];
	pipe(p2c);
	pipe(c2p);
	int pid = fork();

	if(pid < 0){
		fprintf(2, "Fork error!!\n");
		close(p2c[0]);
		close(p2c[1]);
		close(c2p[0]);
		close(c2p[1]);
		exit(1);
	}
	else if(pid == 0){ //child
		close(p2c[1]);
		close(c2p[0]);
		if(read(p2c[0], &buf, 1) != 1){
			fprintf(2, "Child read error!\n");
			exit(1);
		}
		else
			fprintf(1, "%d: received ping\n", getpid()); 

		if(write(c2p[1], &buf, 1) != 1){
			fprintf(2,"Child write error!\n");
			exit(1);
		}
		close(p2c[0]);
		close(c2p[1]);
	}
	else{ //parent
		close(p2c[0]);
		close(c2p[1]);
		if(write(p2c[1], &buf, 1) != 1){
			fprintf(2, "Parent write error!\n");
			exit(1);
		}

		if(read(c2p[0], &buf, 1) != 1){
			fprintf(2, "Parent read error!\n"); 
			exit(1);
		}
		else
			fprintf(1, "%d: received pong\n", getpid());
		
		close(p2c[1]);
		close(c2p[0]);
	}
	exit(0);
}
