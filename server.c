#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include <limits.h>
#include "libft/libft.h"

static int	clientpid;

void	registerpid(int *result, unsigned int *base)
{
	clientpid = *result;
	*result = 0;
	*base = 128;
}

void	reset(unsigned int *base, int *result, int *cont)
{
	if (*cont == 40 && *result != 0)
	{
		write(1, &*result, 1);
		*base = 128;
		*cont = 32;
		*result = 0;
	}
	else if (*cont == 40 && *result == 0)
	{
		*cont = 0;
		*base = 2147483648;
	}
}

void	conv_txt(int bit)
{
	static unsigned int		base = 2147483648;
	static int				result = 0;
	static int				cont = 0;

	if (cont < 32)
	{
		if (bit == SIGUSR1)
			result = result + base;
		base = base / 2;
	}
	if (cont == 31)
		registerpid(&result, &base);
	if (cont >= 32 && cont <= 39)
	{
		if (bit == SIGUSR1)
			result = result + base;
		base = base / 2;
		kill(clientpid, SIGUSR1);
	}
	cont++;
	if (result == 0 && cont == 40)
		reset(&base, &result, &cont);
	if (cont == 40)
		reset(&base, &result, &cont);
}

int	main(void)
{
	int	control;

	control = 0;
	ft_putnbr(getpid());
	write(1, "\n", 1);
	signal(SIGUSR1, conv_txt);
	signal(SIGUSR2, conv_txt);
	pause();
	while (1)
		sleep(1);
	return (0);
}