/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:30:28 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/17 15:03:41 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "libft.h"

int	ft_strncmp(const char *f, const char *s, size_t length)
{
	size_t	i;

	i = 0;
	while (f[i] == s[i] && f[i] != '\0' && i < length)
		i++;
	if (i == length)
		return (0);
	return (f[i] - s[i]);
}
/*
void compare_strncmp(const char *f, const char *s, size_t length) {
    int result1 = ft_strncmp(f, s, length);
    int result2 = strncmp(f, s, length);
    printf("Comparando \"%s\" y \"%s\" con length %zu: ft_strncmp: %d,
	strncmp: %d\n", f, s, length, result1, result2);
}

int main() {
    // Casos de prueba
    compare_strncmp("abc", "abcd", 3);
    compare_strncmp("abc", "abcde", 5);
    compare_strncmp("abcdef", "abc", 3);
    compare_strncmp("abc", "abz", 2);
    compare_strncmp("abc", "abcd", 4);
    compare_strncmp("abcd", "abc", 4);
	compare_strncmp("la casa loca", "la casa loca de antuel", 25);
    
    return 0;
}
int main(void)
{
	char *a = "panza y tacos";
	char *b = "panza y tacos dsafdsa";

	printf("comparao %s con %s", a, b);
	printf("\n resultado: %d\n", ft_strncmp(a, b, 17));
	return(0);
}
*/