/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:27:48 by llabatut          #+#    #+#             */
/*   Updated: 2025/08/13 17:41:42 by anoviedo         ###   ########.fr       */
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
		&& ft_strncmp(cmds->args[0], "exit", 4) == 0)
	{
		if (ft_exit(cmds->args, my_envp, exit_code) == 0)
		{
			fcf(cmds);
			return (1);
		}
	}
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
		input = read_full_input(*my_envp);
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

/*
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes
--tool=massif --pages-as-heap=yes --track-origins=yes --trace-children=yes
--suppressions=a.supp --log-file=val_log.txt ./minishell
*/

// Affiche le contenu de la structure t_cmd pour debug
/*void	print_cmd(t_cmd *cmd)
{
	int	i;

valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes
--suppressions=a.supp --log-file=val_log.txt ./minishell

*/
