/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 17:08:53 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 15:03:19 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	sizesrc;

	sizesrc = 0;
	while (src[sizesrc])
		sizesrc++;
	if (size > 0)
	{
		i = 0;
		while (i < size - 1 && src[i])
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (sizesrc);
}
/*
int	main(void)
{
	const char	*str;
	char		destino[2] = "nada";

	str = "hola como estas gato";
	printf("en la variable destino hay: %s\n", destino);
	printf("tamano de str = %zu\n", ft_strlcpy(destino, str, sizeof(destino)));
	printf("en la variable destino ahora hay: %s\n", destino);
	return (0);
}
*/