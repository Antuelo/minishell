/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:45:19 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/15 17:47:37 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	ft_putnr_fd(int n, int fd)
{
	char	c;

	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n >= 10)
		ft_putnr_fd(n / 10, fd);
	c = (n % 10) + '0';
	write (fd, &c, 1);
}
/*
int main (void)
{
	int a = -2147483648;
	int b = 42;
	int c = -42;
	int d = 0;
	int e = 2147483647;
	ft_putnr_fd(a, 1);
	write(1, "\n", 1);
	ft_putnr_fd(b, 1);
	write(1, "\n", 1);
	ft_putnr_fd(c, 1);
	write(1, "\n", 1);
	ft_putnr_fd(d, 1);
	write(1, "\n", 1);
	ft_putnr_fd(e, 1);
	write(1, "\n", 1);
	return (0);
}
*/