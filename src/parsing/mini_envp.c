/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <anoviedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 11:22:38 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/07 11:22:49 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"
# include "parsing.h"

// Affiche le prompt, lit l'entrée utilisateur, gère l'historique
char	*get_user_input(void)
{
	char	*line;

	line = readline("minishell$ ");
	if (!line)
	{
		write(1, "exit\n", 5);
		exit(0);
	}
	if (line[0])
		add_history(line);
	return (line);
}

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
>>>>>>>> origin/exec-dev:src/parsing/mini_envp.c
}
