/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:43:51 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 20:43:51 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	free_cmd_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

// Libère la structure t_cmd
void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (cmd->delimiter)
		free(cmd->delimiter);
	if (cmd->args)
		free_cmd_args(cmd->args);
	if (cmd->heredoc)
	{
		if (cmd->hdoc_pipe[0] != -1)
			close(cmd->hdoc_pipe[0]);
		if (cmd->hdoc_pipe[1] != -1)
			close(cmd->hdoc_pipe[1]);
	}
	free(cmd);
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_cmd(cmd);
		cmd = tmp;
	}
}

void	free_all(char *line, t_token *tokens, t_cmd *cmds)
{
	if (line)
		free(line);
	if (tokens)
		free_tokens(tokens);
	if (cmds)
		free_cmd_list(cmds);
}
