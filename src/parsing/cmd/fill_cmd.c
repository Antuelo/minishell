/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 19:30:26 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 19:30:26 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
		if (curr->type == T_WORD
			&& !(curr->prev && curr->prev->type >= T_REDIR_IN
				&& curr->prev->type <= T_HEREDOC)
			&& !copy_argument(cmd, curr->value, &i))
			return (0);
		if (curr->type >= T_REDIR_IN && curr->type <= T_HEREDOC
			&& !handle_redir_fail(cmd, curr, i))
			return (0);
		if (curr->type >= T_REDIR_IN && curr->type <= T_HEREDOC)
			curr = curr->next;
		curr = curr->next;
	}
	cmd->args[i] = NULL;
	return (1);
}
