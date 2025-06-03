/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:44:09 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 20:44:09 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// Affiche le prompt, lit l'entrée utilisateur, gère l'historique
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
