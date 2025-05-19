/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:26:28 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/19 19:26:28 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// Vérifie si un caractère peut faire partie d'un nom de variable ($VAR)
static int	is_var_char(char c)
{
	return (isalnum(c) || c == '_');
}

// Remplace toutes les variables dans une chaîne (ex: $USER, $?) par leur valeur
char	*expand_var(char *str, char **envp, int exit_status)
{
	char	tmp[4096]; // Buffer temporaire pour construire la nouvelle string
	char	*result;
	char	*var;
	char	*val;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			// Expansion spéciale de $?
			if (str[i + 1] == '?')
			{
				val = ft_itoa(exit_status); // convertit le code de retour en string
				strcpy(&tmp[j], val);
				j += strlen(val);
				free(val);
				i += 2;
			}
			else
			{
				int start = ++i;
				// Trouve le nom de la variable (lettres/chiffres/_)
				while (is_var_char(str[i]))
					i++;
				var = strndup(&str[start], i - start);
				val = get_env_value(var, envp); // Cherche la valeur dans envp
				strcpy(&tmp[j], val);
				j += strlen(val);
				free(var);
			}
		}
		else
			tmp[j++] = str[i++]; // Caractère normal : copie tel quel
	}
	tmp[j] = '\0';
	result = strdup(tmp); // Duplique la string finale
	return (result);
}

// Cherche la valeur d'une variable d'environnement (ex: PATH) dans envp
char	*get_env_value(char *name, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = strlen(name);
	while (envp[i])
	{
		// Si le nom matche et qu'il est suivi d'un '=' : retourne la valeur
		if (!strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (""); // Si la variable n'existe pas : retourne une string vide
}

// Applique expand_var() à chaque token de type T_WORD contenant un $
void	expand_tokens(t_token *tokens, char **envp, int exit_status)
{
	t_token	*curr;
	char	*expanded;

	curr = tokens;
	while (curr)
	{
		if (curr->type == T_WORD && curr->value && strchr(curr->value, '$'))
		{
			expanded = expand_var(curr->value, envp, exit_status);
			free(curr->value);
			curr->value = expanded;
		}
		curr = curr->next;
	}
}
