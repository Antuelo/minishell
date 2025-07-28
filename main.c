/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:25:29 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/28 17:25:29 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

#define CLR_GREEN "\001\033[1;32m\002"
#define CLR_RESET "\001\033[0m\002"

int			g_exit_status;

int	second_control(t_cmd *cmds, char ***my_envp, int *exit_code)
{
	if (cmds && cmds->args && ft_strncmp(cmds->args[0], "exit", 4) == 0)
	{
		if (ft_exit(cmds->args, my_envp, exit_code) == 0)
		{
			free_cmd_list(cmds);
			return (1); // signaler à main qu’on doit quitter
		}
	}
	if (cmds)
	{
		execute_pipeline(cmds, my_envp);
		free_cmd_list(cmds);
	}
	return (0);
}

static char	*first_control(char **my_envp)
{
	char	*prompt;
	char	*input;

	(void)my_envp; // si plus utilisé ici
	prompt = CLR_GREEN "minishell$ " CLR_RESET;
	input = readline(prompt);
	if (!input)
	{
		write(1, "exit\n", 5);
		return (NULL);
	}
	return (input);
}

/* rl_catch_signals = 0; c'est pour pouvoir controler moi même
** le hendler, c'est à dire, les signals... EN REALITé DESACTIVE
** LES HANDLERS INTERNES
** 	signal(SIGINT, handle_signs) = installe mon prope handler pour (ctrl + c)
*/
int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmds;
	char	*input = NULL;
	char	**my_envp;
	int		exit_code = 0;

	g_exit_status = 0;
	(void)argc;
	(void)argv;
	my_envp = copy_envp(envp);
	rl_catch_signals = 0;
	signal(SIGINT, handle_signs);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = first_control(my_envp);
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			cmds = parse_line(input, my_envp, g_exit_status);
			if (second_control(cmds, &my_envp, &exit_code))
			{
				free(input);
				break ;
			}
		}
		else
			cmds = NULL;
		free(input);
	}
	quit_minishell(my_envp, exit_code);
	return (0);
}

/*

valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=a.supp --log-file=val_log.txt ./minishell

*/
