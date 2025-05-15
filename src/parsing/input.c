/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:45:23 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/15 18:45:23 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

char	*get_user_input(void)
{
	char	*line;

	line = readline("minishell$ ");
	if (!line)
	{
		write(1, "exit\n", 5);
		exit(0);
	}
	if (line[0])
		add_history(line);
	return (line);
}
