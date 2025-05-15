/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:59:15 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/15 14:13:24 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>

typedef struct s_cmd
{
	char			**args;      // ["grep", "foo", NULL]
	char			*infile;     // NULL si pas de <
	char			*outfile;    // NULL si pas de > ou >>
	int				append;       // 1 si >>, 0 si >, -1 si aucune redirection
	int				heredoc;      // 1 si heredoc, 0 sinon
	char			*delimiter;  // Pour heredoc : ex. "EOF"
	int				hdoc_pipe[2]; // Pipe pour le heredoc
	struct s_cmd	*next;
	struct s_cmd	*prev;
}					t_cmd;

#endif
