#include "../../include/parsing.h"

static int	is_var_char(char c)
{
	return (isalnum(c) || c == '_');
}

char	*expand_var(char *str, char **envp, int exit_status)
{
	char	*result;
	int		i;
	int		j;
	char	*var;
	char	*val;
	char	tmp[4096];

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			if (str[i + 1] == '?')
			{
				val = ft_itoa(exit_status);
				strcpy(&tmp[j], val);
				j += strlen(val);
				free(val);
				i += 2;
			}
			else
			{
				int start = ++i;
				while (is_var_char(str[i]))
					i++;
				var = strndup(&str[start], i - start);
				val = get_env_value(var, envp);
				strcpy(&tmp[j], val);
				j += strlen(val);
				free(var);
			}
		}
		else
			tmp[j++] = str[i++];
	}
	tmp[j] = '\0';
	result = strdup(tmp);
	return (result);
}

char	*get_env_value(char *name, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = strlen(name);
	while (envp[i])
	{
		if (!strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (""); // si non trouvée, remplacer par chaîne vide
}

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

