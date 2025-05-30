/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin_exec4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:54:23 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/30 18:51:11 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Met à jour les variables d’environnement PWD et OLDPWD
** après un changement de répertoire réussi avec cd.
** OLDPWD prend l’ancienne valeur de PWD,
** et PWD est mis à jour avec le chemin actuel (getcwd).
*/
void	update_pwd_vars(char ***envp)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*str_old;
	char	*str_new;

	old_pwd = get_env_value(*envp, "PWD");
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return ;
	str_old = ft_strjoin("OLDPWD=", old_pwd);
	str_new = ft_strjoin("PWD=", new_pwd);
	add_or_replace_var(envp, str_old);
	add_or_replace_var(envp, str_new);
	free(new_pwd);
	free(str_new);
	free(str_old);
}

/*c'est seulement pour imprimer le PWD dasn le cas
** où tout se passe bien et le deuxième argument
** c'est un "-". Tout pour la norminette...*/
void	print_in_case(char *arg, char *path)
{
	if (!arg)
		return ;
	if (ft_strncmp(arg, "-", 2) == 0)
		printf("%s\n", path);
}
