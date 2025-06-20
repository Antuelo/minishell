/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 12:51:04 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 15:01:27 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

void	move(unsigned char *d, const unsigned char *s, size_t n)
{
	size_t	i;

	if (d < s)
	{
		i = 0;
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	else
	{
		while (n > 0)
		{
			n--;
			d[n] = s[n];
		}
	}
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	if (d == s || n == 0)
		return (dest);
	move (d, s, n);
	return (dest);
}
/*
int main(void)
{
    char str[50] = "12";
	char str2[20] = "abc def";
    
    printf("Antes de memmove: %s\n", str);
    
    memmove(str, str2, 6);
    
    printf("Después de memmove: %s\n\n", str);
    
    return 0;
}
*/