/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:01:28 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/19 19:01:28 by llabatut         ###   ########.ch       */
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
	T_HEREDOC       // 5, <<
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;





char	*get_user_input(void);
t_token	*tokenize(char *line);
t_token	*new_token(char *value, t_token_type type);
void	free_tokens(t_token *tokens);
void	print_tokens(t_token *tokens);
int	handle_redirection(t_cmd *cmd, t_token *curr);
int	fill_cmd_from_tokens(t_token *tokens, t_cmd *cmd);
int	count_args(t_token *tokens);
char	*get_env_value(char *name, char **envp);
char	*expand_var(char *str, char **envp, int exit_status);
void	expand_tokens(t_token *tokens, char **envp, int exit_status);
char	*ft_itoa(int n);
#endif