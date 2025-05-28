/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:15:12 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/27 22:15:12 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// Remplit une structure t_cmd à partir d'une liste de tokens
int	fill_cmd_from_tokens(t_token *tokens, t_cmd *cmd)
{
	t_token	*curr;
	int		arg_count;
	int		i;

	curr = tokens;
	i = 0;

	arg_count = count_args(tokens);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (0);
	while (i <= arg_count)
		cmd->args[i++] = NULL; // Défensif

	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->delimiter = NULL;
	cmd->append = -1;
	cmd->heredoc = 0;

	i = 0;
	curr = tokens;
	while (curr)
	{
		if (curr->type == T_WORD)
		{
			// On saute les arguments de redirection (ex : fichier après >)
			if (curr->prev && curr->prev->type >= T_REDIR_IN && curr->prev->type <= T_HEREDOC)
			{
				curr = curr->next;
				continue ;
			}
			cmd->args[i] = strdup(curr->value);
			if (!cmd->args[i++])
				return (0);
		}
		else if (curr->type >= T_REDIR_IN && curr->type <= T_HEREDOC)
		{
			handle_redirection(cmd, curr); // Supposé valide (déjà checké)
			curr = curr->next; // Skip du mot cible (ex : > fichier.txt)
		}
		curr = curr->next;
	}
	cmd->args[i] = NULL;
	return (1);
}



// Compte le nombre d'arguments valides
int	count_args(t_token *tokens)
{
	t_token	*curr;
	int		count;

	count = 0;
	curr = tokens;
	while (curr)
	{
		if (curr->type == T_WORD)
		{
			// Si ce T_WORD est lié à une redirection (ex: fichier après >), on le compte pas
			if (curr->prev
				&& (curr->prev->type == T_REDIR_IN
					|| curr->prev->type == T_REDIR_OUT
					|| curr->prev->type == T_REDIR_APPEND
					|| curr->prev->type == T_HEREDOC))
			{
				curr = curr->next;
				continue ;
			}
			count++;
		}
		curr = curr->next;
	}
	return (count);
}
