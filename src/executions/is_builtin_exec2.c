/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin_exec2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:55:04 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/30 21:11:36 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*comparation de l'argument(ex USER) plus le "=" pour chercher considences*/
int	comparison_envp(char **args, char **envp, int i, int j)
{
	size_t	len;

	len = ft_strlen(args[j]);
	if (ft_strncmp(envp[i], args[j], len) == 0
		&& (envp[i][len] == '=' || envp[i][len] == '\0'))
		return (1);
	else
		return (0);
}

/*fontion pour unset ... compte combien de modification pour
**pouvoir faire malloc...
** combien de variables NON dois pas eliminer*/
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

/*fonction pour unset ... genère un nouveux envp modifié
donc, si on a éliminé des paramètres*/
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

/*bubble sort pour imprimer de façon alphabetique le env
demandé dans le built-in export*/
void	bubble_sort(char **env)
{
	int		i;
	int		j;
	char	*temp;
	int		count;

	count = 0;
	while (env[count])
		count++;
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strncmp(env[j], env[j + 1], ft_strlen(env[j])) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

/*trier et imprimer env pour export quand il n y a
** que export comme argument seulement...*/
void	tryed_env(char **envp)
{
	char	**copy;
	int		i;

	copy = copy_envp(envp);
	if (!copy)
		return ;
	bubble_sort(copy);
	i = 0;
	while (copy[i])
	{
		print_export_format(copy[i]);
		i++;
	}
	free_envp(copy, i);
}

