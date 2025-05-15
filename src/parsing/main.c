/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:45:29 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/15 18:45:29 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"


void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (cmd->delimiter)
		free(cmd->delimiter);
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free(cmd);
}

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

int	main(void)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmd;

	while (1)
	{
		line = get_user_input();
		if (!line[0])
		{
			free(line);
			continue ;
		}
		tokens = tokenize(line);
		if (!tokens)
		{
			free(line);
			continue ;
		}
		cmd = malloc(sizeof(t_cmd));
		if (!cmd)
			return (free_tokens(tokens), free(line), 1);
		if (!fill_cmd_from_tokens(tokens, cmd))
			printf("Parsing failed.\n");
		else
			print_cmd(cmd);
		free_cmd(cmd);
		free_tokens(tokens);
		free(line);
	}
	return (0);
}