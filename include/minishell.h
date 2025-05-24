/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:59:15 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/24 10:47:21 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern int			g_exit_status;

typedef struct s_cmd
{
	char			**args;      // ["grep", "foo", NULL]
	char			*infile;     // NULL si pas de <
	char			*outfile;    // NULL si pas de > ou >>
	int				append;       // 1 si >>, 0 si >, -1 si aucune redirection
	int				heredoc;      // 1 si heredoc, 0 sinon
	char			*delimiter;  // Pour heredoc : ex. "EOF, end, finish ..."
	int				hdoc_pipe[2]; // Pipe pour le heredoc
	structs_cmd		*next;
	struct s_cmd	*prev;
}					t_cmd;

typedef struct s_exec
{
	int				pipe_fd[2]; // pour le pipe actuel
	int				fd_in;      // fd d entrée pour le procces actuel
	pid_t			*pid;     // array pour les procces fils
	int				cmd_count;  // cantité de commandes
}					t_exec;

/*execution and main*/
int					execute(t_cmd *cmd, char **envp);
char				*get_cmd_path(char *cmd, char **envp);
int					execute_pipeline(t_cmd *cmd_list, char **envp);
int					is_builtin(char *cmd);
int					exec_builtin(t_cmd *cmd, char **envp);
char				**copy_envp(char **envp);
int					count_env(char **envp);
int					ft_echo(char **args);
int					ft_env(char **envp);
int					ft_pwd(void);
int					ft_unset(char **args, char **envp);
int					ft_exit(char **args);
char				ft_export(char **args, char **envp);
int					ft_cd(char **args, char **envp);

/*utils*/
int					countcmds(t_cmd *cmd);
void				execute_path(char *path, char **envp, char **args,
						char *cmd);
char				**extract_paths(char **envp);
void				controlpath(char *path, t_cmd *cmd);
void				execute_execve(char *fullpath, t_cmd *cmd, char **envp);
void				handle_infile(t_cmd *cmd);
void				handle_outfile(t_cmd *cmd);

/*free everythings*/
void				freepath(char **patch);
void				free_cmd(t_cmd *cmd);
void				free_envp(char **envp, int count);
void				wait_all_processes(t_exec *exec);

#endif
