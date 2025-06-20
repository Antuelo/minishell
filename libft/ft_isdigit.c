/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 22:45:11 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 15:01:54 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int h)
{
	return (h >= '0' && h <= '9');
}

/*
#include <stdio.h>

int	main(void) {
	char c = 'l';  // Podés cambiar el valor para probar otros caracteres

	if (ft_isdigit(c))
	{
		printf("El carácter '%c' es un dígito.\n", c);
	} else {
		printf("El carácter '%c' no es un dígito.\n", c);
	}
	return (0);
}
*/