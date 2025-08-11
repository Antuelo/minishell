/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:50:21 by llabatut          #+#    #+#             */
/*   Updated: 2025/08/08 14:50:21 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

char	*read_full_input(char **envp)
{
	char	*input;
	char	*next_line;
	t_token	*tokens;

	input = first_control(envp);
	if (!input)
		return (NULL);
	tokens = tokenize(input);
	while (is_incomplete_line(tokens))
	{
		free_tokens(tokens);
		next_line = readline("> ");
		if (!next_line)
		{
			free(input);
			return (NULL);
		}
		input = strjoin_and_free(input, next_line);
		tokens = tokenize(input);
	}
	free_tokens(tokens);
	return (input);
}
