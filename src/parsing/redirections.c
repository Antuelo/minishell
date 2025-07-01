/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:14:34 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/02 00:18:01 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

// Gère la redirection d'entrée (ex: < fichier)
// Remplace infile si elle existe déjà
static int	set_infile(t_cmd *cmd, t_token *token)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = strdup(token->value);
	if (!cmd->infile)
		return (0);
	cmd->append = -1;
	return (1);
}

// Gère la redirection de sortie (ex: > ou >>)
// Remplace outfile si elle existe déjà et stocke si append ou non
static int	set_outfile(t_cmd *cmd, t_token *token, int append)
{
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = strdup(token->value);
	if (!cmd->outfile)
		return (0);
	cmd->append = append;
	return (1);
}

// Gère les heredoc (ex: << EOF)
// Stocke uniquement le délimiteur pour traitement plus tard
static int	set_heredoc(t_cmd *cmd, t_token *token)
{
	int		n;
	int		i;
	char	**tmp;

	n = 0;
	i = -1;
	if (!cmd || !token || !token->value)
		return (0);
	while (cmd->delimiter && cmd->delimiter[n])
		n++;
	tmp = malloc(sizeof(char *) * (n + 2));
	if (!tmp)
		return (0);
	while (++i < n)
		tmp[i] = cmd->delimiter[i];
	tmp[n] = ft_strdup(token->value);
	tmp[n + 1] = NULL;
	free(cmd->delimiter);
	cmd->delimiter = tmp;
	cmd->heredoc = 1;
	return (1);
}

// Analyse une redirection et appelle la fonction adaptée
int	handle_redirection(t_cmd *cmd, t_token *curr)
{
	t_token	*next;

	next = curr->next;
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

/*static int	set_heredoc(t_cmd *cmd, t_token *token)
{
	if (!cmd || !token || !token->value)
		return (0);
	cmd->heredoc = 1;
	if (cmd->delimiter)
	{
		free(cmd->delimiter);
		cmd->delimiter = NULL;
	}
	cmd->delimiter = strdup(token->value);
	if (!cmd->delimiter)
		return (0);
	return (1);
}*/
