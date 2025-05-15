/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:39:17 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/15 19:02:35 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell/include/minishell.h"

int	exec(t_cmd *data, char **envp)
{
	char	*fullpath;

	fullpath = get_cmd_path(data->args, envp);
	return (0);
}
