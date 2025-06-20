/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 22:44:51 by anoviedo          #+#    #+#             */
/*   Updated: 2024/10/21 13:21:02 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "libft.h"
//parecido a strcmp, solo que no tiene en cuenta '\0',
// y compara de forma binaria (no por ascii)
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;
	char	*first;
	char	*second;	

	i = 0;
	first = (char *)s1;
	second = (char *)s2;
	while (i < n && first[i] == second[i])
		i++;
	if (i == n)
		return (0);
	return (first[i] - second[i]);
}

/*
void test_memcmp()
{
    char str1[] = "hola";
    char str2[] = "hola";
    char str3[] = "holb";
    char str4[] = "holaa";
    char str5[] = "ho";
    char str6[] = "hola mundo";

    printf("Comparando '%s' y '%s': %d\n", str1, str2, ft_memcmp(str1, str2, 4));
    printf("Comparando '%s' y '%s': %d\n", str1, str3, ft_memcmp(str1, str3, 4));
    printf("Comparando '%s' y '%s': %d\n", str1, str4, ft_memcmp(str1, str4, 5));
    printf("Comparando '%s' y '%s': %d\n", str1, str5, ft_memcmp(str1, str5, 2));
    printf("Comparando '%s' y '%s': %d\n", str1, str6, 
	ft_memcmp(str1, str6, 10));

    // Comparar con la función estándar memcmp
    printf("\nComparaciones con memcmp:\n");
    printf("Comparando '%s' y '%s': %d\n", str1, str2, memcmp(str1, str2, 4));
    printf("Comparando '%s' y '%s': %d\n", str1, str3, memcmp(str1, str3, 4));
    printf("Comparando '%s' y '%s': %d\n", str1, str4, memcmp(str1, str4, 5));
    printf("Comparando '%s' y '%s': %d\n", str1, str5, memcmp(str1, str5, 2));
    printf("Comparando '%s' y '%s': %d\n", str1, str6, memcmp(str1, str6, 10));
}

int main()
{
    test_memcmp();
    return 0;
}
*/