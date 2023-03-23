#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[MAXARG])
{
	int i;
	char *new_argv[MAXARG]={0};
	char buf[512], *p;
	for(i = 0; i < argc; i++)
		new_argv[i] = argv[i];
	new_argv[i]=buf;
	p = buf;
	i = 0;
	while(read(0, p, 1))
	{
		if(*p == '\n')
		{
			*p='\0';
			if(fork() == 0)
			{
				argc++;
				exec(new_argv[1], new_argv+1);
				fprintf(2, "exec %s failed\n", new_argv[0]);
				exit(1);
			}
			else
			{
				wait((int *)0);
				p = buf;
			}
		}
		else p++;
	}
	
	exit(0);
}
