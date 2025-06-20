/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <anoviedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:41:59 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/20 10:44:40 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

int	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		ft_putendl_fd(envp[i++], 1);
	return (0);
}

/* pour la norminette j'ai du eliminer ça, et les declarer avec la fonction
int	i;
	int	j;
	int	new_line;

	i = 1;
	new_line = 1;*/
int	ft_echo(char **args, int i, int j, int new_line)
{
	while (args[1] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (!args[i][j])
		{
			new_line = 0;
			i++;
			if (!args[i])
				return (0);
		}
		else
			break ;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i++], 1);
		if (args[i])
			write(1, " ", 1);
	}
	if (new_line)
		write(1, "\n", 1);
	return (0);
}

int	exec_builtin(t_cmd *cmd, char ***envp)
{
	int	builtin_id;

	builtin_id = is_builtin(cmd->args[0]);
	if (builtin_id == 0)
		return (0);
	else if (builtin_id == 1)
		return (ft_echo(cmd->args, 1, 1, 1));
	else if (builtin_id == 2)
		return (ft_env(*envp));
	else if (builtin_id == 3)
		return (ft_pwd());
	else if (builtin_id == 4)
		return (ft_unset(cmd->args, envp));
	else if (builtin_id == 5)
		return (ft_exit(cmd->args));
	else if (builtin_id == 6)
		return (ft_export(cmd->args, envp));
	else if (builtin_id == 7)
		return (ft_cd(cmd->args, envp));
	else
		return (-1);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (-1);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (2);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (3);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (4);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (5);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (6);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (7);
	return (0);
}
