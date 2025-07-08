/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:06:17 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/08 14:07:08 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

// Compte le nombre d'arguments valides
static int	count_args(t_token *tokens)
{
	t_token	*curr;
	int		count;

	count = 0;
	curr = tokens;
	while (curr)
	{
		if (curr->type == T_WORD)
		{
			if (curr->prev && (curr->prev->type == T_REDIR_IN
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

// Alloue le tableau d'arguments de la commande en comptant les T_WORD valides
// fonction utilitaire de fill_cmd_from_tokens
int	allocate_args_array(t_token *tokens, t_cmd *cmd)
{
	int	arg_count;
	int	i;

	arg_count = count_args(tokens);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (0);
	i = 0;
	while (i <= arg_count)
		cmd->args[i++] = NULL;
	return (1);
}

// Initialise les champs d'entrée/sortie et redirections d'une commande
// fonction utilitaire de fill_cmd_from_tokens
void	init_cmd_fields(t_cmd *cmd)
{
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->delimiter = NULL;
	cmd->append = -1;
	cmd->heredoc = 0;
}

// Copie un argument dans le tableau cmd->args, avec gestion des erreurs
// fonction utilitaire de fill_cmd_from_tokens
int	copy_argument(t_cmd *cmd, char *value, int *i)
{
	cmd->args[*i] = strdup(value);
	if (!cmd->args[*i])
	{
		while (*i > 0)
		{
			(*i)--;
			free(cmd->args[*i]);
		}
		free(cmd->args);
		cmd->args = NULL;
		return (0);
	}
	(*i)++;
	return (1);
}

// Appelle handle_redirection et libère les ressources cmd en cas d'échec
// fonction utilitaire de fill_cmd_from_tokens
/* fill_cmd_utils.c */
int	handle_redir_fail(t_cmd *cmd, t_token *curr)
{
	int	i;

	if (handle_redirection(cmd, curr))
		return (1);
	if (cmd->delimiter)
	{
		i = 0;
		while (cmd->delimiter[i])
			free(cmd->delimiter[i++]);
		free(cmd->delimiter);
		cmd->delimiter = NULL;
	}
	cmd->heredoc = 0;
	return (0);
}

/*int	handle_redir_fail(t_cmd *cmd, t_token *curr)
{
	if (!handle_redirection(cmd, curr))
	{
		if (cmd->delimiter)
		{
			free(cmd->delimiter);
			cmd->delimiter = NULL;
		}
		return (0);
	}
	return (1);
}*/
