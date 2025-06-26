/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:43:45 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/23 22:22:10 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"
# include "parsing.h"

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

// Boucle principale d’expansion des variables dans une string
static void	expansion_loop(const char *str, t_expand_ctx *ctx, int g_exit_status)
{
	while (str[*ctx->i] && ctx->buffer)
	{
		if (str[*(ctx->i)] == '\\' && str[*(ctx->i) + 1] == '$')
			handle_escaped_dollar(ctx->buffer, ctx->j, ctx->i);
		else if (str[*(ctx->i)] == '$' && str[*(ctx->i) + 1])
		{
			if (str[*(ctx->i) + 1] == '?')
				handle_exit_code(ctx->buffer, ctx->j, ctx->i, g_exit_status);
			else
				handle_env_variable(ctx, str);
		}
		else
			ctx->buffer[(*ctx->j)++] = str[(*ctx->i)++];
	}
}

// Remplace les variables ($VAR, $?) dans une string par leur valeur
static char	*expand_var(const char *str, char **envp, int g_exit_status)
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
	expansion_loop(str, &ctx, g_exit_status);
	buffer[j] = '\0';
	return (ft_strdup(buffer));
}

// Applique l’expansion des variables à chaque token, sauf en single quotes
void	expand_tokens(t_token *tokens, char **envp, int g_exit_status)
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
			expanded = expand_var(curr->value, envp, g_exit_status);
			free(curr->value);
			curr->value = expanded;
			if (expanded[0] == '\0')
				tokens = remove_empty_token(curr, tokens);
		}
		curr = next;
	}
}
