/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:03:59 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/20 21:05:37 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

// Vérifie si un token est un opérateur de redirection (<, <<, >, >>)
static int	is_redir(t_token *t)
{
	return (t->type >= T_REDIR_IN && t->type <= T_HEREDOC);
}

// Traite une redirection : vérifie la présence d'un mot après et l'applique
static int	process_redirection(t_token **curr, t_cmd *cmd, int i)
{
	if (!(*curr)->next || (*curr)->next->type != T_WORD)
		return (0);
	if (!handle_redir_fail(cmd, *curr, i))
		return (0);
	*curr = (*curr)->next;
	return (1);
}

// Copie un mot dans la liste d'arguments si ce n’est pas une cible de redirection
static int	process_word(t_token *curr, t_cmd *cmd, int *i)
{
	if (!(curr->prev && is_redir(curr->prev)))
		if (!copy_argument(cmd, curr->value, i))
			return (0);
	return (1);
}

// Remplit une structure t_cmd à partir d’une portion de tokens
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
		if (curr->type == T_WORD && !process_word(curr, cmd, &i))
			return (0);
		else if (is_redir(curr) && !process_redirection(&curr, cmd, i))
			return (0);
		curr = curr->next;
	}
	cmd->args[i] = NULL;
	if (!cmd->args[0] && !cmd->infile && !cmd->outfile)
		return (printf("Syntax error: empty command\n"), 0);
	return (1);
}
