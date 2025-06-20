/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:25:14 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 15:04:15 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	slen;

	slen = 0;
	while (s[slen] != '\0')
		slen++;
	if (start == slen)
		return ((char *)malloc(1));
	if (start > slen)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (s[start + i] != '\0' && start < slen && i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
/*
int	main(void)
{
	char	*result;

	// Caso 1: Cadena normal, extraer desde el medio
	result = ft_substr("Hello, World!", 7, 5);
	printf("Caso 1: %s\n", result);
	free(result);
	// Caso 2: Extraer toda la cadena desde el principio
	result = ft_substr("Hello, World!", 0, 12);
	printf("Caso 2: %s\n", result);
	free(result);


// Debería devolver una cadena vacía?...
	// Caso 3: start es mayor que la longitud de la cadena
	result = ft_substr("Hello", 10, 5);
	printf("Caso 3: %s\n", result);
	free(result);
	// Caso 4: len es mayor que los caracteres disponibles desde el start
	result = ft_substr("Hello", 3, 10);
	printf("Caso 4: %s\n", result);
	free(result);
	// Caso 5: Extraer un solo carácter
	result = ft_substr("Hello", 1, 1);
	printf("Caso 5: %s\n", result);
	free(result);
	return (0);
}
*/