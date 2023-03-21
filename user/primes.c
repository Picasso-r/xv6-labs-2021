#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void filter(int left)
{
	int prime, num;
	int p[2];
	pipe(p);
	//int pid = fork();
	if(read(left, &prime, sizeof(prime)) != 0)
		fprintf(1, "prime %d\n", prime);
	else 
		exit(0);
	while(read(left, &num, sizeof(num)))
	{
		if(num % prime == 0)
			continue;
		write(p[1], &num, sizeof(num));
	}
	int pid = fork();
	if(pid < 0)
	{
        fprintf(2, "Fork error!\n");
        close(p[0]);
        close(p[1]);
		exit(1);
	}	
	else if(pid == 0)
	{
		close(p[1]);
		filter(p[0]);
		close(p[0]);
		exit(0);
	}
	else
	{
		close(p[0]);
		/*
		while(read(left, &num, sizeof(num)))
		{
			//fprintf(1,"Process %d handling %d\n", getpid(), num);
			if(num % prime == 0)
				continue;
			write(p[1], &num, sizeof(num));
		}
		*/
		close(p[1]);
		wait((int *)0);
	}
	//exit(0);
}

int main(int argc, char **argv)
{
	int i;
	int p[2];
	pipe(p);
	for(i = 2; i <= 35; i++)
		write(p[1], &i, sizeof(i));
	int pid = fork();
	if(pid < 0)
	{
		fprintf(2, "Fork error!\n");
		close(p[0]);
		close(p[1]);
		exit(1);
	}
	else if(pid == 0)
	{
		//sleep(10);
		close(p[1]);
		filter(p[0]);
		close(p[0]);
	}
	/*else
	{
		close(p[0]);
		for(i = 2; i <= 35; i++)
			write(p[1], &i, sizeof(i));
`		close(p[1]);
	}*/
	else {
		close(p[0]);
		close(p[1]);
		wait((int *)0);

	}
	exit(0);
}
