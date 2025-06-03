/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 21:19:20 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 21:20:26 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// Remplace $? par la valeur du dernier code de sortie dans buffer
void	handle_exit_code(char *buffer, int *j, int *i, int exit_status)
{
	char	*val;

	val = ft_itoa(exit_status);
	ft_strcpy(&buffer[*j], val);
	*j += ft_strlen(val);
	free(val);
	*i += 2;
}

// Vérifie si un caractère peut faire partie d'un nom de variable
static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

// Remplace une variable d’environnement par sa valeur dans buffer
void	handle_env_variable(t_expand_ctx *ctx, const char *str)
{
	int		start;
	char	*var;
	char	*val;

	(*ctx->i)++;
	start = *ctx->i;
	while (is_var_char(str[*ctx->i]))
		(*ctx->i)++;
	var = ft_substr(str, start, *ctx->i - start);
	val = get_env_value(var, ctx->envp);
	if (val)
	{
		ft_strcpy(&ctx->buffer[*ctx->j], val);
		*ctx->j += ft_strlen(val);
	}
	free(var);
}

// Supprime un token vide de la liste chaînée et retourne la nouvelle tête
t_token	*remove_empty_token(t_token *curr, t_token *tokens)
{
	if (curr->prev)
		curr->prev->next = curr->next;
	if (curr->next)
		curr->next->prev = curr->prev;
	if (curr == tokens)
		tokens = curr->next;
	free(curr->value);
	free(curr);
	return (tokens);
}

// Gère l’échappement de $ en insérant un $ littéral dans buffer
void	handle_escaped_dollar(char *buffer, int *j, int *i)
{
	buffer[*j] = '$';
	(*j)++;
	*i += 2;
}
