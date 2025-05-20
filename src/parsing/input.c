/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:26:35 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/19 19:26:35 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// Affiche le prompt, lit l'entrée utilisateur, gère l'historique et Ctrl+D
char	*get_user_input(void)
{
	char	*line;

	// Affiche le prompt et récupère l'entrée utilisateur
	line = readline("minishell$ ");
	if (!line)
	{
		// Si l'utilisateur fait Ctrl+D, readline renvoie NULL : on quitte
		write(1, "exit\n", 5);
		exit(0);
	}
	// Si la ligne n'est pas vide : on l'ajoute à l'historique
	if (line[0])
		add_history(line);
	return (line);
}
