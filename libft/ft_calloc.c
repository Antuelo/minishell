/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:20:02 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 15:01:57 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*mem;
	size_t	i;

	i = 0;
	mem = (char *)malloc(nmemb * size);
	if (mem == NULL)
		return (NULL);
	while (i < (nmemb * size))
	{
		mem[i] = '\0';
		i++;
	}
	return ((void *)mem);
}
