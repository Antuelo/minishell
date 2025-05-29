/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin_exec2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:55:04 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/29 13:46:03 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*comparation de l'argument plus le = pour chercher considences*/
int	comparison_envp(char **args, char **envp, int i, int j)
{
	if (ft_strncmp(envp[i], args[j], ft_strlen(args[j])) == 0
		&& envp[i][ft_strlen(args[j])] == '=')
		return (1);
	else
		return (0);
}

/*fontion pour unset ... compte combien de modification pour malloc*/
int	count_to_keep(char **args, char **envp, int i, int j)
{
	int	count;
	int	skip;

	count = 0;
	while (envp[i])
	{
		skip = 0;
		j = 1;
		while (args[j])
		{
			if (comparison_envp(args, envp, i, j))
			{
				skip = 1;
				break ;
			}
			j++;
		}
		if (!skip)
			count++;
		i++;
	}
	return (count);
}

/*fonction pour unset ... genère un nouveux envp modifié*/
char	**rebuild_envp(char **args, char **envp, int i, int j)
{
	char	**new_envp;
	int		k;
	int		skip;

	new_envp = malloc(sizeof(char *) * (count_to_keep(args, envp, 0, 0) + 1));
	if (!new_envp)
		return (NULL);
	k = 0;
	while (envp[i])
	{
		skip = 0;
		j = 1;
		while (args[j] && !skip)
			skip = comparison_envp(args, envp, i, j++);
		if (!skip)
			new_envp[k++] = ft_strdup(envp[i]);
		i++;
	}
	new_envp[k] = NULL;
	return (new_envp);
}
