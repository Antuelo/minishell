/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 21:58:41 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/21 23:31:09 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_unset(char **args, char **envp)
{
	return (0);
}

int	ft_exit(char **args)
{
	exit(0);
}

char	ft_export(char **args, char **envp)
{
	return (NULL);
}

int	ft_cd(char **args, char **envp)
{
	return ;
}
