/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 19:32:15 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 19:32:15 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_cmd_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

// Libère la structure t_cmd
void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (cmd->delimiter)
		free(cmd->delimiter);
	if (cmd->args)
		free_cmd_args(cmd->args);
	// Fermer les pipes heredoc si ouverts
	if (cmd->heredoc)
	{
		if (cmd->hdoc_pipe[0] != -1)
			close(cmd->hdoc_pipe[0]);
		if (cmd->hdoc_pipe[1] != -1)
			close(cmd->hdoc_pipe[1]);
	}
	free(cmd);
}


// Affiche le contenu de la structure t_cmd pour debug
void	print_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	printf("Infile    : %s\n", cmd->infile);
	printf("Outfile   : %s\n", cmd->outfile);
	printf("Append    : %d\n", cmd->append);
	printf("Heredoc   : %d\n", cmd->heredoc);
	printf("Delimiter : %s\n", cmd->delimiter);
	printf("Args      : ");
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
			printf("[%s] ", cmd->args[i++]);
		printf("\n");
	}
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd *tmp;
	while (cmd)
	{
		tmp = cmd->next;
		free_cmd(cmd);
		cmd = tmp;
	}
}
void	free_all(char *line, t_token *tokens, t_cmd *cmds)
{
	if (line)
		free(line);
	if (tokens)
		free_tokens(tokens);
	if (cmds)
		free_cmd_list(cmds);
}



int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;
	int		last_exit_code;

	(void)argc;
	(void)argv;
	last_exit_code = 0;

	while (1)
	{
		line = get_user_input();
		if (!line)
			continue;

		if (!line[0])
		{
			free(line);
			continue;
		}

		if (check_unclosed_quotes(line))
		{
			printf("Syntax error: unclosed quote\n");
			free(line);
			continue;
		}

		tokens = tokenize(line);
		if (!tokens)
		{
			free(line);
			continue;
		}

		if (!syntax_is_valid(tokens))
		{
			free_all(line, tokens, NULL);
			continue;
		}

		expand_tokens(tokens, envp, last_exit_code);
		remove_quotes_from_tokens(tokens);

		cmds = build_cmd_list_from_tokens(tokens);
		if (!cmds)
		{
			printf("Parsing failed.\n");
			free_all(line, tokens, NULL);
			continue;
		}

		t_cmd *tmp = cmds;
		while (tmp)
		{
			print_cmd(tmp);
			printf("--------\n");
			tmp = tmp->next;
		}

		free_all(line, tokens, cmds); 
	}
	return (0);
}
