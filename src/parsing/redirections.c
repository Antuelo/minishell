/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:27:06 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/19 19:27:06 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// Gère la redirection d'entrée (ex: < fichier)
// Remplace infile si elle existe déjà
static int	set_infile(t_cmd *cmd, t_token *token)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = strdup(token->value);
	cmd->append = -1; // aucune redirection de sortie
	return (1);
}

// Gère la redirection de sortie (ex: > ou >>)
// Remplace outfile si elle existe déjà et stocke si append ou non
static int	set_outfile(t_cmd *cmd, t_token *token, int append)
{
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = strdup(token->value);
	cmd->append = append;
	return (1);
}

// Gère les heredoc (ex: << EOF)
// Stocke uniquement le délimiteur pour traitement plus tard
static int	set_heredoc(t_cmd *cmd, t_token *token)
{
	cmd->heredoc = 1;
	cmd->delimiter = strdup(token->value);
	return (1);
}

// Analyse une redirection et appelle la fonction adaptée
int	handle_redirection(t_cmd *cmd, t_token *curr)
{
	t_token	*next;

	next = curr->next;

	// Vérifie qu’un fichier ou délimiteur suit la redirection
	if (!next || next->type != T_WORD)
	{
		printf("Syntax error: expected filename after redirection\n");
		return (0);
	}

	// Selon le type de redirection, appelle la fonction adaptée
	if (curr->type == T_REDIR_IN)
		return (set_infile(cmd, next));
	if (curr->type == T_REDIR_OUT)
		return (set_outfile(cmd, next, 0));
	if (curr->type == T_REDIR_APPEND)
		return (set_outfile(cmd, next, 1));
	if (curr->type == T_HEREDOC)
		return (set_heredoc(cmd, next));

	return (1);
}
