/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:29:45 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/10 20:32:53 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static int	skip_redirs_and_check_filename(t_token **next)
{
	while (*next && ((*next)->type == T_REDIR_IN
			|| (*next)->type == T_REDIR_OUT
			|| (*next)->type == T_REDIR_APPEND
			|| (*next)->type == T_HEREDOC))
	{
		if (!(*next)->next || (*next)->next->type != T_WORD)
		{
			printf("Syntax error: expected filename\n");
			return (0);
		}
		*next = (*next)->next->next;
	}
	return (1);
}

int	check_pipe_redirection(t_token *tokens)
{
	t_token	*curr;
	t_token	*next;

	curr = tokens;
	while (curr)
	{
		if (curr->type == T_PIPE && curr->next)
		{
			next = curr->next;
			if (!skip_redirs_and_check_filename(&next))
				return (0);
			if (!next)
			{
				printf("Syntax error: missing command after pipe\n");
				return (0);
			}
		}
		curr = curr->next;
	}
	return (1);
}
