/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin_exec1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 21:58:41 by anoviedo          #+#    #+#             */
/*   Updated: 2025/07/07 20:35:50 by anoviedo         ###   ########.fr       */
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
	new_envp = rebuild_envp(args, *envp, 0);
	if (!new_envp)
		return (free_envp(*envp, count), 1);
	free_envp(*envp, count);
	*envp = new_envp;
	return (0);
}

int	ft_exit(char **args)
{
	long	exit_code;
	int		i;

	write(2, "exit\n", 5);
	if (!args[1])
		exit(0);
	i = 0;
	while (args[1][i])
	{
		if ((args[1][i] < '0' || args[1][i] > '9') && !(i == 0
				&& args[1][i] == '-'))
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
	exit_code = ft_atoi(args[1]);
	exit((unsigned char)exit_code);
}

int	ft_export(char **args, char ***envp)
{
	int	i;

	i = 1;
	if (!args || !args[1])
		return (tryed_env(*envp), 0);
	while (args[i])
	{
		if (!is_valid_key(args[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			return (1);
		}
		if (add_or_replace_var(envp, args[i]))
			return (1);
		i++;
	}
	return (0);
}

int	ft_cd(char **args, char ***envp)
{
	char	*path;

	if (!args[1])
		path = get_env_value("HOME", *envp);
	else if (ft_strncmp(args[1], "-", 2) == 0)
		path = get_env_value("OLDPWD", *envp);
	else
		path = args[1];
	if (!path || path[0] == '\0')
	{
		if (!args[1] || ft_strncmp(args[1], "-", 2) != 0)
			printf("minishell: cd: HOME not set\n");
		else
			printf("minishell: cd: OLDPWD not set\n");
		return (1);
	}
	if (chdir(path) != 0)
	{
		printf("minishell: cd: %s: \n", path);
		return (perror(""), 1);
	}
	else
		update_pwd_vars(envp);
	print_in_case(args[1], path);
	return (0);
}
