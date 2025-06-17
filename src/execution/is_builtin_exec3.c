/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin_exec3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:20:19 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/17 11:30:17 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

/*fonction pour export, imitation print de bash*/
void	print_export_format(char *line)
{
	int	i;

	i = 0;
	write(1, "declare -x ", 11);
	while (line[i] && line[i] != '=')
		write(1, &line[i++], 1);
	if (line[i] == '=')
	{
		write(1, "=\"", 2);
		i++;
		while (line[i])
			write(1, &line[i++], 1);
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

/*fonction pour export, control de l'argument*/
int	is_valid_key(char *args)
{
	int	i;

	if (!args || !args[0])
		return (0);
	if (!(ft_isalpha(args[0]) || args[0] == '_'))
		return (0);
	i = 1;
	while (args[i] && args[i] != '=')
	{
		if (!(ft_isalnum(args[i]) || args[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/*fonction pour export, ajute une nouvelle variable à env*/
int	add_new_var(char ***envp, char *new_var)
{
	char	**new_envp;
	int		count;
	int		i;

	count = count_env(*envp);
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (1);
	i = 0;
	while (i < count)
	{
		new_envp[i] = (*envp)[i];
		i++;
	}
	new_envp[i++] = ft_strdup(new_var);
	new_envp[i] = NULL;
	free(*envp);
	*envp = new_envp;
	return (0);
}

/*pour export ... remplacer si existe et cumple toutes les requis
ajouter si elle n existe pas*/
int	add_or_replace_var(char ***envp, char *new_var)
{
	int		i;
	char	*key;

	i = 0;
	while (new_var[i] && new_var[i] != '=')
		i++;
	key = ft_substr(new_var, 0, i);
	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], key, ft_strlen(key)) == 0
			&& ((*envp)[i][ft_strlen(key)] == '='
				|| (*envp)[i][ft_strlen(key)] == '\0'))
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strdup(new_var);
			free(key);
			return (0);
		}
		i++;
	}
	if (add_new_var(envp, new_var))
		return (free(key), 1);
	free(key);
	return (0);
}

/*
J UTILISE L AUTRE
fonction pour cd ... charche dans env ce qui est apres le "="
ex. key = "PWD" et le return = /home/user
char	*get_env_value(char **envp, char *key)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(key);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}*/
