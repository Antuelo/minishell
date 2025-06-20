/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 16:12:53 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 15:01:20 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

void	*ft_memcpy(void *restrict dst, const void *restrict src, size_t n)
{
	unsigned char	*h;
	unsigned char	*i;

	h = (unsigned char *)src;
	i = (unsigned char *)dst;
	while (n)
	{
		*i = *h;
		i++;
		h++;
		n--;
	}
	return (dst);
}
