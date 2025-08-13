/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_pars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:09:53 by llabatut          #+#    #+#             */
/*   Updated: 2025/08/13 17:47:33 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

void	f_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

// Libère une structure t_cmd et tous ses champs dynamiques
void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (cmd->delimiter)
		f_array(cmd->delimiter);
	if (cmd->args)
		f_array(cmd->args);
	if (cmd->heredoc)
	{
		if (cmd->hdoc_pipe[0] != -1)
			close(cmd->hdoc_pipe[0]);
		if (cmd->hdoc_pipe[1] != -1)
			close(cmd->hdoc_pipe[1]);
	}
	free(cmd);
}

// Libère une liste chaînée de t_cmd
void	fcl(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		free_cmd(cmd);
		cmd = next;
	}
}

// Libère line, tokens et la liste de commandes cmds
void	free_all(char *line, t_token *tokens, t_cmd *cmds)
{
	if (line)
		free(line);
	if (tokens)
		free_tokens(tokens);
	if (cmds)
		fcl(cmds);
}

/*fcf = free_cmd_full --- > coupé pour passer la norminette*/
void	fcf(t_cmd *any)
{
	if (!any)
		return ;
	while (any->prev)
		any = any->prev;
	fcl(any);
}
