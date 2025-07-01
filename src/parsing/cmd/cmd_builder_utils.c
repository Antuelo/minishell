/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builder_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:10:46 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/01 19:10:50 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

// Met à jour les pointeurs head/last pour chaîner
// la nouvelle commande dans la liste
// fonction utilitaire de process_command
static void	update_cmd_links(t_cmd **head, t_cmd **last, t_cmd *new)
{
	if (!*head)
		*head = new;
	else
		(*last)->next = new;
	*last = new;
}

// Initialise une nouvelle commande et remplit ses champs à partir des tokens
// fonction utilitaire de process_command
static int	handle_new_cmd(t_token *start, t_token *end, t_cmd **new)
{
	*new = init_cmd();
	if (!*new)
		return (0);
	if (!fill_cmd_from_tokens(start, end, *new))
	{
		free_cmd(*new);
		return (0);
	}
	(*new)->next = NULL;
	return (1);
}

// Gère la création d'une commande à partir d'un segment de tokens
// et l'ajoute à la liste
int	process_command(t_cmd **head, t_cmd **last,
				t_token **start, t_token *curr)
{
	t_cmd	*new;
	t_token	*limit;
	int		ok;

	limit = NULL;
	if (curr->type == T_PIPE)
		limit = curr;
	ok = handle_new_cmd(*start, limit, &new);
	if (!ok)
		return (0);
	update_cmd_links(head, last, new);
	if (curr->type == T_PIPE)
		*start = curr->next;
	return (1);
}
