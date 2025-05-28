/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:16:41 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/28 16:16:41 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// Vérifie si un caractère peut faire partie d'un nom de variable
static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

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

// Remplace les $VAR et $? par leur valeur
char	*expand_var(const char *str, char **envp, int exit_status)
{
	char	buffer[4096];
	char	*result;
	int		i = 0, j = 0;

	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] == '$')
		{
			buffer[j++] = '$';
			i += 2;
		}
		else if (str[i] == '$' && str[i + 1])
		{
			if (str[i + 1] == '?')
			{
				char *val = ft_itoa(exit_status);
				ft_strcpy(&buffer[j], val);
				j += ft_strlen(val);
				free(val);
				i += 2;
			}
			else
			{
				int start = ++i;
				while (is_var_char(str[i]))
					i++;
				char *var = ft_substr(str, start, i - start);
				char *val = get_env_value(var, envp);
				ft_strcpy(&buffer[j], val);
				j += ft_strlen(val);
				free(var);
			}
		}
		else
			buffer[j++] = str[i++];
	}
	buffer[j] = '\0';
	result = ft_strdup(buffer);
	return (result);
}

// Applique expand_var à tous les T_WORD sauf ceux dans des single quotes
void	expand_tokens(t_token *tokens, char **envp, int exit_status)
{
	t_token	*curr;
	char	*expanded;

	curr = tokens;
	while (curr)
	{
		t_token *next = curr->next;
		// Expansion seulement sur les T_WORD qui ne sont pas dans des quotes simples
		if (curr->type == T_WORD
			&& curr->value
			&& strchr(curr->value, '$')
			&& !curr->in_single_quote)
		{
			expanded = expand_var(curr->value, envp, exit_status);
			free(curr->value);
			curr->value = expanded;

			// Si le résultat est une chaîne vide, on supprime le token
			if (expanded[0] == '\0')
			{
				// Retirer curr de la liste
				if (curr->prev)
					curr->prev->next = curr->next;
				if (curr->next)
					curr->next->prev = curr->prev;

				// Si c'était le premier token, mettre à jour la tête
				if (curr == tokens)
					tokens = curr->next;

				free(curr->value);
				free(curr);
			}
		}
		curr = next;
	}
}