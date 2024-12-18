/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:16:59 by nyoong            #+#    #+#             */
/*   Updated: 2024/12/17 21:30:15 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include "libft/libft.h"
#include "libft/ft_printf.h"

static int	g_bit_count;

void	sendpid(unsigned int c, int pidserv)
{
	unsigned int	base;
	unsigned int	cont;

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
		signal(SIGUSR1, confirm);
		usleep(6000);
	}
	usleep(4000);
}

void	confirm()
{
	g_bit_count++;
	ft_printf("Received bit %d\n", g_bit_count);
}

int	main(int argc, char **argv)
{
	int	pidserv;
	int	i;

	i = 0;
	g_bit_count = 0;
	if (argc != 3)
	{
		ft_printf("Usage: ./client [PID] [message]\n");
		return (-1);
	}
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
