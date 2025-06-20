/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:15:11 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/21 13:21:02 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

int	jump(const char *nptr)
{
	int	i;

	i = 0;
	if (nptr[i] == '\0')
		return (0);
	while (nptr[i] == '\t' || nptr[i] == '\n' || nptr[i] == '\v'
		|| nptr[i] == '\f' || nptr[i] == '\r' || nptr[i] == ' ')
		i++;
	return (i);
}

int	ft_atoi(const char *nptr)
{
	int	num;
	int	neg;
	int	i;

	num = 0;
	neg = 0;
	i = jump (nptr);
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			neg = 1;
		i++;
	}
	while (nptr[i] && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		num = (num * 10) + nptr[i] - '0';
		i++;
	}
	if (neg == 1)
		num = -num;
	return (num);
}
/*
int	main(void) {
	// Pruebas
	printf("%d\n", ft_atoi("   -123ugui"));   // -123
	printf("%d\n", ft_atoi("456"));        // 456
	printf("%d\n", ft_atoi("   +789abc")); // 789
	printf("%d\n", ft_atoi("abc123"));     // 0
	printf("%d\n", ft_atoi("   "));        // 0
	return (0);
}
*/