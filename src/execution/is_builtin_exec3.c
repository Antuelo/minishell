/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin_exec3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:30:21 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/25 13:31:46 by llabatut         ###   ########.ch       */
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
static int	is_valid_key(char *args)
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
static int	add_new_var(char ***envp, char *new_var)
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

int	ft_export(char **args, char ***envp, t_cmd *cmd)
{
	int	i;

	if (cmd && cmd->next)
		return (0);
	i = 1;
	if (!args || !args[1])
		return (tryed_env(*envp), 0);
	while (args[i])
	{
		if (!is_valid_key(args[i]))
		{
			ft_putstr_fd("export: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			return (1);
		}
		add_or_replace_var(envp, args[i]);
		i++;
	}
	return (0);
}
