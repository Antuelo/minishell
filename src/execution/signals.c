/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 20:16:06 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/18 20:17:23 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

/*	rl_replace_line("", 0) nettoie ce qui a fait l utilisateur
** 	rl_on_new_line(); c'est pour informer que on es dans une nouvelle ligne
	rl_redisplay(); pour reimprimer le prompt
*/
void	handle_signs(int sign)
{
	(void)sign;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
