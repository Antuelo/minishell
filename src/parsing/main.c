/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:12:53 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/12 18:12:53 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	main(void)
{
	char	*line;
	t_token	*tokens;

	while (1)
	{
		line = get_user_input();
		if (!line[0])
		{
			free(line);
			continue ;
		}
		tokens = tokenize(line);
		print_tokens(tokens);
		free_tokens(tokens);
		free(line);
	}
	return (0);
}
