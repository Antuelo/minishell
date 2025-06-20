/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 21:34:57 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 15:02:22 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

int	wordmax(const char *s, char c)
{
	int	max;
	int	size;

	max = 0;
	size = 0;
	while (*s != '\0')
	{
		if (*s != c)
			size++;
		else if (size > 0)
		{
			if (size > max)
				max = size;
			size = 0;
		}
		s++;
	}
	if (size > max)
		max = size;
	return (max);
}

char	**asigmall(size_t raw, int max)
{
	char	**str;
	size_t	fila;

	str = (char **)malloc(sizeof(char *) * (raw + 1));
	if (str == NULL)
		return (NULL);
	fila = 0;
	while (fila < raw)
	{
		str[fila] = (char *)malloc(sizeof(char) * (max + 1));
		if (str[fila] == NULL)
		{
			fila--;
			while (fila > 0)
			{
				free(str[fila]);
				fila--;
			}
			free(str);
			return (NULL);
		}
		fila++;
	}
	str[fila] = NULL;
	return (str);
}

char	**copytab(char **tab, char const *str, char c)
{
	size_t	h;
	size_t	i;
	size_t	j;

	j = 0;
	h = 0;
	while (str[h] != '\0')
	{
		i = 0;
		while (str[h] == c)
			h++;
		if (str[h] != c && str[h] != '\0')
		{
			while (str[h] != c && str[h] != '\0')
			{
				tab[j][i] = str[h];
				i++;
				h++;
			}
			tab[j][i] = '\0';
			j++;
		}
	}
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	int		max;
	char	**str;

	if (s == NULL || *s == '\0' || c == '\0')
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
		{
			j++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
	}
	max = wordmax(s, c);
	str = asigmall(j, max);
	if (!str)
		return (NULL);
	return (copytab(str, s, c));
}
/*
int	main(void)
{
	char	*str = "--0123- dfs- - dsfa--1234567890-----";
	char	c = '-';
	char	**result;
	size_t	i = 0;

	result = ft_split(str, c);
	while (result && result[i])
	{
		printf("Parte %zu: %s\n", i, result[i]);
		free(result[i]);
		i++;
	}
	free(result);
	return (0);
}
*/