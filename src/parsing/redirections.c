/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:35:04 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/08 20:35:22 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

// Gère la redirection d'entrée (ex: < fichier)
// Remplace infile si elle existe déjà
static int	set_infile(t_cmd *cmd, t_token *token)
{
	int	fd;

	fd = open(token->value, O_RDONLY);
	if (fd == -1)
	{
		perror(token->value);
		return (0);
	}
	close(fd);
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
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(token->value, flags, 0644);
	if (fd == -1)
	{
		perror(token->value);
		return (0);
	}
	close(fd);
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
