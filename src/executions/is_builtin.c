/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:41:59 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/20 19:23:51 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	int	i;
	int	control;

	i = 0;
	control = 0;
	if (!args || !args[i])
		return (write(1, "\n", 1), 0);
	i = 1;
	while (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
		control = i++;
	while (args[i])
	{
		ft_putstr_fd(args[i++], 1);
		if (args[i])
			write (1, " ", 1);
	}
	if (control == 0)
		write (1, "\n", 1);
	return (0);
}

int	exec_builtin(t_cmd *cmd, char **envp)
{
	int	builtin_id;

	builtin_id = is_builtin(cmd->args[0]);
	if (builtin_id == 0)
		return (1);
	switch (builtin_id)
	{
	case 1:
		return (ft_echo(cmd->args));
	case 2:
		return (ft_env(envp));
	case 3:
		return (ft_pwd());
	case 4:
		return (ft_unset(cmd->args, envp));
	case 5:
		return (ft_exit(cmd->args));
	case 6:
		return (ft_export(cmd->args, envp));
	case 7:
		return (ft_cd(cmd->args, envp));
	default:
		return (1);
	}
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (2);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (3);                       // jusqu'à 3 c'est dans fork
	if (ft_strncmp(cmd, "unset", 6) == 0) //à partir du 4 c'est hors fork
		return (4);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (5);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (6);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (7);
	return (0);
}
