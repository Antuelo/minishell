/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:01:28 by llabatut          #+#    #+#             */
/*   Updated: 2025/08/24 00:17:54 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

#define CLR_GREEN "\001\033[1;32m\002"
#define CLR_RESET "\001\033[0m\002"

int			g_exit_status;

static int	second_control(t_cmd *cmds, char ***my_envp, int *exit_code)
{
	if (cmds && cmds->args && cmds->args[0]
		&& ft_strncmp(cmds->args[0], "exit", 5) == 0)
	{
		*exit_code = ft_exit(cmds->args, my_envp, exit_code);
		if (*exit_code == 0)
			return (fcf(cmds), 1);
		else if (*exit_code == 1)
			return (fcf(cmds), 0);
		else if (*exit_code == 2)
			return (fcf(cmds), 2);
		else
			return (fcf(cmds), *exit_code);
	}
	(void)*exit_code;
	if (cmds)
	{
		execute_pipeline(cmds, my_envp);
		fcf(cmds);
	}
	return (0);
}

char	*first_control(char **my_envp)
{
	char	*prompt;
	char	*input;

	(void)my_envp;
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
void	init_minishell(char **argv, int argc, char **envp, char ***my_envp)
{
	(void)argc;
	(void)argv;
	*my_envp = copy_envp(envp);
	rl_catch_signals = 0;
	signal(SIGINT, handle_signs);
	signal(SIGQUIT, SIG_IGN);
	g_exit_status = 0;
}

void	main_loop(char ***my_envp, int *exit_code)
{
	t_cmd	*cmds;
	char	*input;

	while (1)
	{
		input = first_control(*my_envp);
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			cmds = parse_line(input, *my_envp, g_exit_status);
			if (second_control(cmds, my_envp, exit_code))
			{
				free(input);
				break ;
			}
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**my_envp;
	int		exit_code;

	init_minishell(argv, argc, envp, &my_envp);
	exit_code = 0;
	main_loop(&my_envp, &exit_code);
	quit_minishell(my_envp, exit_code);
	return (0);
}
