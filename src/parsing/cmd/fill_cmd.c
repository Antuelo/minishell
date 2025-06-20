/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 21:04:58 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/17 11:32:48 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

// Remplit une structure t_cmd à partir des tokens entre tokens et limit
// Elle alloue les arguments, initialise les champs,
// copie les T_WORD non liés aux redirections et
// traite les redirections (<, >, <<, >>) en sautant les fichiers/limiteurs.
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
		if (curr->type == T_WORD && !(curr->prev
				&& curr->prev->type >= T_REDIR_IN
				&& curr->prev->type <= T_HEREDOC) && !copy_argument(cmd,
				curr->value, &i))
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
