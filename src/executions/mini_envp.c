/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 22:28:19 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/29 14:57:22 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

char	**copy_envp(char **envp)
{
	int		i;
	int		count;
	char	**copy;

	i = 0;
	count = count_env(envp);
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			free_envp(copy, i);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
