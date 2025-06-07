/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:07:16 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 20:16:52 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_cmd	*cmds;
	int		last_exit_code = 0;

	(void)argc;
	(void)argv;

	while (1)
	{
		line = get_user_input();
		if (!line)
			continue;

		cmds = parse_line(line, envp, last_exit_code);
		if (!cmds)
			continue;

		t_cmd *tmp = cmds;
		while (tmp)
		{
			print_cmd(tmp);
			printf("--------\n");
			tmp = tmp->next;
		}

		free_cmd_list(cmds);
	}
	return (0);
}


/*int	main(int argc, char **argv, char **envp)
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
}*/
