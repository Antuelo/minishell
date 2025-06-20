/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:51:58 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 15:03:13 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	tot;

	i = 0;
	while (i < size && dst[i] != '\0')
		i++;
	j = 0;
	while (src[j] != '\0')
		j++;
	if (j == 0)
		return (i);
	if (size == 0)
		return (j);
	tot = i + j;
	j = 0;
	while (i < size - 1 && src[j] != '\0')
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (tot);
}
/*
int	main(void)
{
	const char	*fuente = "34567";
	char		destino[7] = "12";

	printf("por un lado tenemos %s\n", fuente);
	printf("por otro %s\n\n", destino);
	printf("la cantidad de caracteres es %zu\n\n", \
	ft_strlcat(destino, fuente,
			sizeof(destino)));
	printf("y todo junto  es:\n %s \n\n", destino);
	return (0);
}
*/