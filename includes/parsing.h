/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:48:22 by llabatut          #+#    #+#             */
/*   Updated: 2025/08/08 14:48:22 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>

typedef struct s_cmd	t_cmd;

/* il est deja dans minishell.h
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
}	t_cmd;*/

typedef enum e_token_type
{
	T_WORD, // 0, cmd or arg
	T_PIPE, // 1, pipe |
	T_REDIR_IN, // 2, <
	T_REDIR_OUT, // 3, >
	T_REDIR_APPEND, // 4, >>
	T_HEREDOC, // 5, <<
	T_INVALID = -1,
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				in_single_quote; // 1 si extrait depuis '...'
	int				in_double_quote; // 1 si extrait depuis "..."
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_expand_ctx
{
	char	*buffer;
	int		*j;
	int		*i;
	char	**envp;
}	t_expand_ctx;

typedef struct s_expand_flag
{
	int		i;
	int		j;
	int		sq_flag;
	int		dq_flag;
}	t_expand_flag;

typedef struct s_bufdata
{
	char	*line;
	int		*i;
	char	*buffer;
	int		*j;
	int		*sq_flag;
	int		*dq_flag;
}	t_bufdata;

typedef struct s_wordinfo
{
	char		buffer[4096];
	int			j;
	int			sq_flag;
	int			dq_flag;
}	t_wordinfo;

/* ************************************************************************** */
/*                                  INPUT                                     */
/* ************************************************************************** */

char	*get_user_input(void);

/* ************************************************************************** */
/*                                  LEXING                                    */
/* ************************************************************************** */

t_token	*tokenize(char *line);
t_token	*handle_combined_word(char *line, int *i);
t_token	*handle_operator_token(char *line, int *i);
t_token	*new_token(char *value, t_token_type type);
void	free_tokens(t_token *tokens);
void	free_single_token(t_token *token);
t_token	*remove_empty_token(t_token *curr, t_token *tokens);
int		is_operator(char c);

/* ************************************************************************** */
/*                                 PARSING                                    */
/* ************************************************************************** */

t_cmd	*build_cmd_list_from_tokens(t_token *tokens);
t_cmd	*parse_line(char *line, char **envp, int exit_code);
int		syntax_is_valid(t_token *tokens);
int		check_syntax_errors(t_token *tokens);
int		check_unclosed_quotes(const char *line);
void	remove_quotes_from_tokens(t_token *tokens);
int		contains_forbidden_chars(char *line);
int		check_pipe_redirection(t_token *tokens);
int		is_incomplete_line(t_token *tokens);
char	*strjoin_and_free(char *s1, char *s2);

/* ************************************************************************** */
/*                                 EXPANSION                                  */
/* ************************************************************************** */

t_token	*expand_tokens(t_token *tokens, char **envp, int g_exit_status);
char	*get_env_value(char *name, char **envp);
void	handle_escaped_dollar(char *buffer, int *j, int *i);
void	handle_exit_code(char *buffer, int *j, int *i, int g_exit_status);
void	handle_env_variable(t_expand_ctx *ctx, const char *str);

/* ************************************************************************** */
/*                                CMD SETUP                                   */
/* ************************************************************************** */

t_cmd	*init_cmd(void);
int		fill_cmd_from_tokens(t_token *tokens, t_token *limit, t_cmd *cmd);
int		handle_redirection(t_cmd *cmd, t_token *curr);
int		handle_redir_fail(t_cmd *cmd, t_token *curr);
int		allocate_args_array(t_token *tokens, t_cmd *cmd);
void	init_cmd_fields(t_cmd *cmd);
int		copy_argument(t_cmd *cmd, char *value, int *i);

/* ************************************************************************** */
/*                                CMD LIST BUILDING                           */
/* ************************************************************************** */

t_token	*prepare_pipe_segment(t_token *pipe);
t_cmd	*add_new_cmd(t_token *start, t_token *pipe, t_cmd *head);
int		process_command(t_cmd **head, t_cmd **last,
			t_token **start, t_token *curr);
void	link_pipe_back(t_token *pipe, t_token *start);
void	link_cmd(t_cmd **head, t_cmd **last, t_cmd *new);

/* ************************************************************************** */
/*                                  FREEING                                   */
/* ************************************************************************** */

void	free_cmd(t_cmd *cmd);
void	free_cmd_list(t_cmd *cmd);
void	ft_free_strarray(char **arr);

#endif
