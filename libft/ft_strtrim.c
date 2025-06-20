/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:07:48 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 15:04:09 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

int	isinset(char const s1, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == s1)
			return (1);
		i++;
	}
	return (0);
}

char	*newstr(char *str, char const *s1, size_t start, size_t end)
{
	size_t	i;

	i = 0;
	while (start < end)
	{
		str[i] = s1[start];
		start++;
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*str;

	if (s1 == NULL || set == NULL)
		return (NULL);
	start = 0;
	while (s1[start] != '\0' && (isinset(s1[start], set) != 0))
		start++;
	end = 0;
	while (s1[end] != '\0')
		end++;
	while (end > 0 && (isinset(s1[end - 1], set) != 0))
		end--;
	str = (char *)malloc(sizeof(char) * ((end - start) + 1));
	if (str == NULL)
		return (NULL);
	newstr(str, s1, start, end);
	return (str);
}
/*
int	main(void)
{
	char *palabra = "?!!?casa!?=??!";
	char *set = "!=?";
	char *result;

	result = ft_strtrim(palabra, set);

	printf("%s\n", result);
	free(result);
}
*/