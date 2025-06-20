/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 16:13:31 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 14:52:25 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*h;

	h = (unsigned char *)s;
	while (n)
	{
		*h = '\0';
		h++;
		n--;
	}
}
/*
int	main(void)
{
	char	str[] = "Hola Mundo";

	printf("Antes de ft_bzero: %s\n", str);
	ft_bzero(str, 5);
	printf("Después de ft_bzero: ");
	for (int i = 0; i < sizeof(str); i++) {
		if (str[i] == '\0')
			printf("\\0");
		else
			printf("%c", str[i]);
	}
	printf("\n");
	return (0);
}
*/