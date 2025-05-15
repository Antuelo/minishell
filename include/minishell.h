/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:59:15 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/15 23:02:56 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>


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
void	execute_path(char *path, char **envp, char **args, char *cmd);
void	free_cmd(t_cmd *cmd);

/*free everythings*/
void	freepath(char **patch);
void	free_cmd(t_cmd *cmd);

#endif
