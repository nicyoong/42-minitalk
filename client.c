#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include "./libft/libft.h"

static int bit_count = 0;

void	sendpid(unsigned int c, int pidserv)
{
	unsigned int		base;
	unsigned int		cont;

	cont = 0;
	base = 2147483648;
	if (cont < 32)
	{
		while (base > 0)
		{
			if (c >= base)
			{
				kill(pidserv, SIGUSR1);
				c = c - base;
			}
			else
			{
				kill(pidserv, SIGUSR2);
			}
			base = base / 2;
			usleep(300);
		}
	}
	usleep(500);
	cont++;
}

void	sendchar(unsigned char c, int pidserv)
{
	int		base;

	base = 128;
	while (base > 0)
	{
		if (c >= base)
		{
			kill(pidserv, SIGUSR1);
			c = c - base;
		}
		else
		{
			kill(pidserv, SIGUSR2);
		}
		base = base / 2;
		usleep(300);
	}
	usleep(500);
}


// Signal handler function
void confirm(int sig) {
    if (sig == SIGUSR1) {
        // Increment the bit counter when SIGUSR1 is received
        bit_count++;
        
        // Print the number of received bits
        dprintf(1, "Received bit %d\n", bit_count);  // Print to standard output
    }
}

int	main(int argc, char **argv)
{
	int	pidserv;
	int	i;

	i = 0;
	if (argc != 3)
	{
		printf("Usage: ./client [PID] [message]\n");
		return (-1);
	}
	signal(SIGUSR1, confirm);
	pidserv = ft_atoi(argv[1]);
	sendpid(getpid(), pidserv);
	while (argv[2][i])
	{
		sendchar(argv[2][i], pidserv);
		i++;
	}
	sendchar('\0', pidserv);
	return (0);
}
