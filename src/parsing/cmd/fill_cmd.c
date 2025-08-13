/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:38:44 by llabatut          #+#    #+#             */
/*   Updated: 2025/08/13 17:41:42 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

// Vérifie si un token est un opérateur de redirection (<, <<, >, >>)
static int	is_redir(t_token *t)
{
	return (t->type >= T_REDIR_IN && t->type <= T_HEREDOC);
}

// Copie un mot dans la liste d'arguments if not une cible de redirection
static int	process_word(t_token *curr, t_cmd *cmd, int *i)
{
	if (!(curr->prev && is_redir(curr->prev)))
		if (!copy_argument(cmd, curr->value, i))
			return (0);
	return (1);
}

// Remplit une structure t_cmd à partir d’une portion de tokens
static int	handle_redir_and_advance(t_cmd *cmd, t_token **curr)
{
	if (!(*curr)->next || (*curr)->next->type != T_WORD)
		return (0);
	if (!handle_redir_fail(cmd, *curr))
		return (0);
	*curr = (*curr)->next;
	return (1);
}

int	fill_cmd_from_tokens(t_token *tokens, t_token *limit, t_cmd *cmd)
{
	t_token	*curr;
	int		i;

	if (!allocate_args_array(tokens, cmd))
		return (0);
	init_cmd_fields(cmd);
	curr = tokens;
	i = 0;
	while (curr && curr != limit)
	{
		if (cmd->invalid)
			return (fcf(cmd), 0);
		if (curr->type == T_WORD && !process_word(curr, cmd, &i))
			return (fcf(cmd), 0);
		else if (is_redir(curr) && !handle_redir_and_advance(cmd, &curr))
			return (fcf(cmd), 0);
		curr = curr->next;
	}
	cmd->args[i] = NULL;
	return (1);
}
