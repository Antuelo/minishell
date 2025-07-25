/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <anoviedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:29:38 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/25 17:30:28 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termio.h>
# include <unistd.h>

extern int				g_exit_status;

/*
**args;			// ["grep", "foo", NULL]
*infile;		// NULL si pas de <
*outfile;		// NULL si pas de > ou >>
append;			// 1 si >>, 0 si >, -1 si aucune redirection
heredoc;		// 1 si heredoc, 0 sinon
**delimiter;	// Pour heredoc : ex. "EOF, end, finish ..."
hdoc_pipe[2];	// Pipe pour le heredoc
*/
typedef struct s_cmd
{
	char				**args;
	char				*infile;
	char				*outfile;
	int					append;
	int					heredoc;
	char				**delimiter;
	int					hdoc_pipe[2];
	struct s_cmd		*next;
	struct s_cmd		*prev;
	int					invalid;
}						t_cmd;

/*
pipe_fd[2];		// pour le pipe actuel
fd_in;			// fd d entrée pour le procces suivant
*pid;			// array pour les procces fils
cmd_count;		// cantité de commandes
*/
typedef struct s_exec
{
	int					pipe_fd[2];
	int					fd_in;
	pid_t				*pid;
	int					cmd_count;
}						t_exec;

/*terminal variables (for heredoc and ctrl+c sings)*/
typedef struct termios	t_termios;
int						save_original_terminal_mode(t_termios *original);
void					restore_original_terminal_mode(t_termios *original);
int						wait_for_heredoc(pid_t pid, t_cmd *cmd,
							t_termios *term);
void					reset_readline(void);
int						handle_heredoc_interrupt(t_cmd *cmd, t_termios *term);

/*execution, main ...*/
int						execute(t_cmd *cmd, char ***envp);
int						execute_pipeline(t_cmd *cmd_list, char ***envp);
char					*get_cmd_path(char *cmd, char **envp);
int						control_builtin(t_cmd *cmd_list, char ***envp);
int						init_exec(t_exec *exec, int count);
void					quit_minishell(char **envp, int code);


/*builtins fork et pipes*/
int						is_builtin(char *cmd);
char					**copy_envp(char **envp);
int						count_env(char **envp);
int						exec_builtin(t_cmd *cmd, char ***envp);
int						ft_echo(char **args, int i, int j, int new_line);
int						ft_env(char **envp);
int						ft_pwd(void);
int						ft_unset(char **args, char ***envp);
char					**rebuild_envp(char **args, char **envp, int i, int j);
int						ft_exit(char **args, char ***envp);
int						ft_export(char **args, char ***envp, t_cmd *cmd);
void					tryed_env(char **envp);
int						ft_cd(char **args, char ***envp);
char					*get_env_value(char *name, char **envp);
void					update_pwd_vars(char ***envp);
void					print_in_case(char *arg, char *path);
int						add_or_replace_var(char ***envp, char *new_var);
void					print_export_format(char *line);
int						control_infiles(t_cmd *cmd);
int						heredoc(t_cmd *cmds, int status);
int						control_fork_pipe(t_cmd *cmd, t_exec *exec, int i);
void					execute_fork(t_cmd *cmd, t_exec *exec, char **envp,
							int i);
void					setup_redirections(t_cmd *cmd, t_exec *exec);
void					parent_process(t_exec *exec, t_cmd *cmd);

/*utils*/
int						countcmds(t_cmd *cmd);
void					execute_path(char *path, char **envp, char **args,
							char *cmd);
void					controlpath(char *path, t_cmd *cmd, char **envp);
void					execute_execve(char *fullpath, t_cmd *cmd, char **envp);
int						handle_infile(t_cmd *cmd);
int						handle_outfile(t_cmd *cmd);
void					handle_signs(int signo);
void					clean_exit(t_cmd *cmd, char **envp, int code);

/*free everythings*/
void					freepath(char **patch);
void					free_cmd(t_cmd *cmd);
void					free_envp(char **envp, int count);
void					wait_all_processes(t_exec *exec);

#endif
