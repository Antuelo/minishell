/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:04:47 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 20:04:47 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
} t_cmd;

typedef enum e_token_type
{
	T_WORD,         // 0, cmd or arg
	T_PIPE,         // 1, pipe |
	T_REDIR_IN,     // 2, <
	T_REDIR_OUT,    // 3, >
	T_REDIR_APPEND, // 4, >>
	T_HEREDOC,      // 5, <<
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

char	*get_user_input(void);
t_token	*tokenize(char *line);
t_token	*new_token(char *value, t_token_type type);
void	free_tokens(t_token *tokens);
void	print_tokens(t_token *tokens); ////////////
int	handle_redirection(t_cmd *cmd, t_token *curr);
int	fill_cmd_from_tokens(t_token *tokens, t_token *limit, t_cmd *cmd);
int	count_args(t_token *tokens);
char	*get_env_value(char *name, char **envp);
char	*expand_var(const char *str, char **envp, int exit_status);
void	expand_tokens(t_token *tokens, char **envp, int exit_status);
int	check_unclosed_quotes(const char *line);
void	remove_quotes_from_tokens(t_token *tokens);
t_cmd	*build_cmd_list_from_tokens(t_token *tokens);
void	free_cmd(t_cmd *cmd);
void	free_cmd_list(t_cmd *cmd);
int	syntax_is_valid(t_token *tokens);
int	check_syntax_errors(t_token *tokens);
t_token *next_pipe(t_token *token);
t_cmd   *init_cmd(void);
t_token	*prepare_pipe_segment(t_token *pipe);
t_cmd	*add_new_cmd(t_token *start, t_token *pipe, t_cmd *head);
void	link_pipe_back(t_token *pipe, t_token *start);
void	link_cmd(t_cmd **head, t_cmd **last, t_cmd *new);
t_token	*last_token(t_token *start);
void	handle_exit_code(char *buffer, int *j, int *i, int exit_status);
void	handle_env_variable(t_expand_ctx *ctx, const char *str);
t_token	*remove_empty_token(t_token *curr, t_token *tokens);
void	handle_escaped_dollar(char *buffer, int *j, int *i);
int	allocate_args_array(t_token *tokens, t_cmd *cmd);
void	init_cmd_fields(t_cmd *cmd);
int	copy_argument(t_cmd *cmd, char *value, int *i);
int	handle_redir_fail(t_cmd *cmd, t_token *curr, int i);
int	is_operator(char c);
t_token	*handle_operator_token(char *line, int *i);
t_token	*handle_quoted_token(char *line, int *i);
t_token	*handle_word_token(char *line, int *i);
void	free_single_token(t_token *token);
int	process_command(t_cmd **head, t_cmd **last,
				t_token **start, t_token *curr);
t_cmd	*parse_line(char *line, char **envp, int exit_code);


// libft, à supprimer
char	*ft_itoa(int n);
int	ft_isalnum(int n);
size_t	ft_strlen(const char *s);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strcpy(char *dst, const char *src);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s1);
char	*ft_strchr(const char *s, int c);
void	*ft_calloc(size_t count, size_t size);

#endif