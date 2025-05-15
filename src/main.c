/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:13:51 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/15 19:38:15 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell/include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd;

	(void)argc;
	(void)argv;
	cmd = malloc(sizeof(t_cmd));
	cmd->args = ft_split("ls -la", ' ');
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = -1;
	cmd->heredoc = 0;
	cmd->delimiter = NULL;
	cmd->next = NULL;
	cmd->append = NULL;

	execute(cmd, envp);

	free_cmd(cmd);
	return (0);
}
