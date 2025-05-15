/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:59:15 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/15 20:21:57 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../minishell/libft/libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct s_cmd
{
	char			**args;      // ["grep", "foo", NULL]
	char			*infile;     // NULL si pas de <
	char			*outfile;    // NULL si pas de > ou >>
	int				append;       // 1 si >>, 0 si >, -1 si aucune redirection
	int				heredoc;      // 1 si heredoc, 0 sinon
	char			*delimiter;  // Pour heredoc : ex. "EOF, end, finish ..."
	int				hdoc_pipe[2]; // Pipe pour le heredoc
	struct s_cmd	*next;
	struct s_cmd	*prev;
}					t_cmd;

/*execution*/
int		execute(t_cmd *cmd, char **envp);
char	*get_cmd_path(char *cmd, char **envp);
char	**extract_paths(char **envp);

#endif
