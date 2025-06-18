/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:59:15 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/18 19:59:04 by anoviedo         ###   ########.fr       */
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
# include <signal.h>

extern int			g_exit_status;

/*
**args;			// ["grep", "foo", NULL]
*infile;		// NULL si pas de <
*outfile;		// NULL si pas de > ou >>
append;			// 1 si >>, 0 si >, -1 si aucune redirection
heredoc;		// 1 si heredoc, 0 sinon
*delimiter;		// Pour heredoc : ex. "EOF, end, finish ..."
hdoc_pipe[2];	// Pipe pour le heredoc
*/
typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	char			*delimiter;
	int				hdoc_pipe[2];
	struct s_cmd	*next;
	struct s_cmd	*prev;
}					t_cmd;

/*
pipe_fd[2];		// pour le pipe actuel
fd_in;			// fd d entrée pour le procces suivant
*pid;			// array pour les procces fils
cmd_count;		// cantité de commandes
*/
typedef struct s_exec
{
	int				pipe_fd[2];
	int				fd_in;
	pid_t			*pid;
	int				cmd_count;
}					t_exec;

/*execution, main ...*/
int					execute(t_cmd *cmd, char ***envp);
int					execute_pipeline(t_cmd *cmd_list, char ***envp);
char				*get_cmd_path(char *cmd, char **envp);
int					control_builtin(t_cmd *cmd_list, char ***envp);
int					init_exec(t_exec *exec, int count);

/*builtins*/
int					is_builtin(char *cmd);
char				**copy_envp(char **envp);
int					count_env(char **envp);
int					exec_builtin(t_cmd *cmd, char ***envp);
int					ft_echo(char **args, int i, int j, int new_line);
int					ft_env(char **envp);
int					ft_pwd(void);
int					ft_unset(char **args, char ***envp);
char				**rebuild_envp(char **args, char **envp, int i, int j);
int					ft_exit(char **args);
int					ft_export(char **args, char ***envp);
void				tryed_env(char **envp);
void				print_export_format(char *line);
int					is_valid_key(char *args);
int					add_or_replace_var(char ***envp, char *new_var);
int					ft_cd(char **args, char ***envp);
char				*get_env_value(char *name, char **envp);
void				update_pwd_vars(char ***envp);
void				print_in_case(char *arg, char *path);

/*utils*/
int					countcmds(t_cmd *cmd);
void				execute_path(char *path, char **envp, \
						char **args, char *cmd);
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
