/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:43:45 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 20:43:45 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// Cherche la valeur d'une variable d'environnement dans envp
char	*get_env_value(char *name, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return ("");
}

static void	expansion_loop(const char *str, t_expand_ctx *ctx, int exit_status)
{
	while (str[*ctx->i] && ctx->buffer)
	{
		if (str[*(ctx->i)] == '\\' && str[*(ctx->i) + 1] == '$')
			handle_escaped_dollar(ctx->buffer, ctx->j, ctx->i);
		else if (str[*(ctx->i)] == '$' && str[*(ctx->i) + 1])
		{
			if (str[*(ctx->i) + 1] == '?')
				handle_exit_code(ctx->buffer, ctx->j, ctx->i, exit_status);
			else
				handle_env_variable(ctx, str);
		}
		else
			ctx->buffer[(*ctx->j)++] = str[(*ctx->i)++];
	}
}

static char	*expand_var(const char *str, char **envp, int exit_status)
{
	char			buffer[4096];
	int				i;
	int				j;
	t_expand_ctx	ctx;

	i = 0;
	j = 0;
	ctx.buffer = buffer;
	ctx.i = &i;
	ctx.j = &j;
	ctx.envp = envp;
	expansion_loop(str, &ctx, exit_status);
	buffer[j] = '\0';
	return (ft_strdup(buffer));
}

// Applique expand_var à tous les T_WORD sauf ceux dans des single quotes
void	expand_tokens(t_token *tokens, char **envp, int exit_status)
{
	t_token	*curr;
	t_token	*next;
	char	*expanded;

	curr = tokens;
	while (curr)
	{
		next = curr->next;
		if (curr->type == T_WORD && curr->value
			&& strchr(curr->value, '$') && !curr->in_single_quote)
		{
			expanded = expand_var(curr->value, envp, exit_status);
			free(curr->value);
			curr->value = expanded;
			if (expanded[0] == '\0')
				tokens = remove_empty_token(curr, tokens);
		}
		curr = next;
	}
}
