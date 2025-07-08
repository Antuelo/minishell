/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin_exec1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 21:37:58 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/08 21:38:05 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("pwd"), 1);
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (0);
}

/*je utilise ***envp pour modifier la racine...
"count" necessaire pour eliminer et liberer le vieux envp*/
int	ft_unset(char **args, char ***envp)
{
	int		count;
	char	**new_envp;

	if (!args || !args[1])
		return (0);
	count = count_env(*envp);
	new_envp = rebuild_envp(args, *envp, 0, 0);
	if (!new_envp)
		return (1);
	free_envp(*envp, count);
	*envp = new_envp;
	return (0);
}

int	ft_exit(char **args)
{
	int	i;

	if (isatty(STDIN_FILENO))
		write(2, "exit\n", 5);
	if (!args[1])
		exit(0);
	i = 0;
	while (args[1][i])
	{
		if ((args[1][i] < '0' || args[1][i] > '9') && !(i == 0
				&& (args[1][i] == '-' || args[1][i] == '+')))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			exit(2);
		}
		i++;
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_exit_status = 1;
		return (1);
	}
	exit((unsigned char)ft_atoi(args[1]));
}

int	ft_cd(char **args, char ***envp)
{
	char	*path;

	if (args[1] && args[2])
		return (ft_putendl_fd("cd: too many arguments", 2), 1);
	if (!args[1])
		path = get_env_value("HOME", *envp);
	else if (ft_strncmp(args[1], "-", 2) == 0)
		path = get_env_value("OLDPWD", *envp);
	else
		path = args[1];
	if (!path || path[0] == '\0')
	{
		if (!args[1] || ft_strncmp(args[1], "-", 2) != 0)
			ft_putendl_fd("cd: HOME not set", 2);
		else
			ft_putendl_fd("cd: OLDPWD not set", 2);
		return (1);
	}
	if (chdir(path) != 0)
		return (fprintf(stderr, "cd: %s: %s\n", path, strerror(errno)), 1);
	update_pwd_vars(envp);
	return (print_in_case(args[1], path), 0);
}
