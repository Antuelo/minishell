/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 23:11:01 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 15:03:49 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

// busca un string dentro de otro hasta "len" caracteres
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (little[i] == '\0')
		return ((char *)big);
	while (big[i] != '\0' && i < len)
	{
		j = 0;
		while ((big[i + j] == little[j] && (i + j) < len))
		{
			if (little[j + 1] == '\0')
				return ((char *)&big[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}
/*
void	test_ft_strnstr(void)
{
	const char	*haystack = "Hola mundo, 123123123123";
	const char	*needle1 = "123123";
	const char	*needle2 = "esto";
	const char	*needle3 = "noexiste";
	const char	*needle4 = "";
	const char	*needle5 = "Hola";

	// Pruebas de la función ft_strnstr
	printf("Prueba 1: '%s' buscando '%s' -> %s\n",
	haystack, needle1, ft_strnstr(haystack, needle1, 17));
	printf("Prueba 2: '%s' buscando '%s' -> %s\n",
	haystack, needle2, ft_strnstr(haystack, needle2, 20));
	printf("Prueba 3: '%s' buscando '%s' -> %s\n",
	haystack, needle3, ft_strnstr(haystack, needle3, 20));
	printf("Prueba 4: '%s' buscando '%s' -> %s\n",
	haystack, needle4, ft_strnstr(haystack, needle4, 20));
	printf("Prueba 5: '%s' buscando '%s' -> %s\n",
	haystack, needle5, ft_strnstr(haystack, needle5, 20));
}

int	main(void)
{
	test_ft_strnstr();
	return (0);
}
*/