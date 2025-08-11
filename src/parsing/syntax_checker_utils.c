/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:49:49 by llabatut          #+#    #+#             */
/*   Updated: 2025/08/08 14:49:59 by llabatut         ###   ########.ch       */
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
		}
		curr = curr->next;
	}
	return (1);
}

char	*strjoin_and_free(char *s1, char *s2)
{
	char	*joined;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 2);
	if (!joined)
		return (NULL);
	ft_memcpy(joined, s1, len1);
	joined[len1] = '\n';
	ft_memcpy(joined + len1 + 1, s2, len2);
	joined[len1 + len2 + 1] = '\0';
	free(s1);
	free(s2);
	return (joined);
}

/*int	check_pipe_redirection(t_token *tokens)
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
			if (!next || next->type != T_WORD)
			{
				printf("Syntax error: missing command after pipe\n");
				return (0);
			}
		}
		curr = curr->next;
	}
	return (1);
}*/
