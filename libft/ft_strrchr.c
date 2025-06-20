/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:57:31 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 15:04:02 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t		i;
	const char	*last;

	i = 0;
	while (s[i] != '\0')
		i++;
	last = &s[i];
	while (i > 0)
	{
		i--;
		if (s[i] == (char)c)
			return ((char *)&s[i]);
	}
	if (i == 0)
		return ((char *)last);
	return (NULL);
}
/*
int main() {
    const char *str = "Hola mundo";
    
    // Pruebas
    printf("Última ocurrencia de 'o': %s\n", ft_strrchr(str, 'o')); // "o"
    printf("Última ocurrencia de 'x': %s\n", ft_strrchr(str, 'x')); // NULL
    printf("Última ocurrencia de ' ': %s\n", ft_strrchr(str, ' ')); // " mundo"
    printf("Última ocurrencia de '\\0': %s\n",
	ft_strrchr(str, '\0')); // Debe devolver el puntero al final
    printf("Última ocurrencia de '': %s\n", ft_strrchr(str, 'l')); // ""


    return 0;
}
*/