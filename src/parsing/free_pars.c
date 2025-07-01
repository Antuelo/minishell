/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_pars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:09:53 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/02 00:20:00 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

void	ft_free_strarray(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

// Libère chaque argument d’un tableau de strings (args)
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

// Libère une liste chaînée de t_cmd
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

// Libère line, tokens et la liste de commandes cmds
void	free_all(char *line, t_token *tokens, t_cmd *cmds)
{
	if (line)
		free(line);
	if (tokens)
		free_tokens(tokens);
	if (cmds)
		free_cmd_list(cmds);
}
