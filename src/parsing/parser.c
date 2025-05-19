/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:26:59 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/19 19:26:59 by llabatut         ###   ########.ch       */
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

	// Compte le nombre d'arguments à allouer pour args[]
	arg_count = count_args(tokens);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (0);

	// Initialisation des champs de redirection
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->delimiter = NULL;
	cmd->append = -1;
	cmd->heredoc = 0;

	// Parcours de la liste de tokens
	while (curr)
	{
		if (curr->type == T_WORD)
		{
			// Si le mot est lié à une redirection (ex: fichier après >), on l'ignore comme argument
			if (curr->prev
				&& (curr->prev->type == T_REDIR_IN
					|| curr->prev->type == T_REDIR_OUT
					|| curr->prev->type == T_REDIR_APPEND
					|| curr->prev->type == T_HEREDOC))
			{
				curr = curr->next;
				continue ;
			}
			// Sinon, on l'ajoute aux args[]
			cmd->args[i++] = strdup(curr->value);
		}
		else if (curr->type >= T_REDIR_IN && curr->type <= T_HEREDOC)
		{
			// Si c’est une redirection, on la gère avec handle_redirection()
			if (!handle_redirection(cmd, curr))
				return (0);
			curr = curr->next; // skip le token suivant (le fichier/delimiter)
		}
		curr = curr->next;
	}
	cmd->args[i] = NULL; // Fin du tableau d'arguments
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
