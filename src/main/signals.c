/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 23:26:49 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 15:19:32 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal;

void	execution_handler(int signal)
{
	g_signal = 1;
	if (signal == SIGINT)
	{
		write(STDIN_FILENO, "\n", 1);
		// if (!DEBUG)
		// {
		// 	rl_on_new_line();
		// 	rl_replace_line("", 0);
		// 	rl_redisplay();
		// }
	}
	if (signal == SIGQUIT)
	{
		ft_putstr_fd("Quit: 3\n", 2);
		// if (!DEBUG)
		// {
		// 	rl_on_new_line();
		// 	rl_replace_line("", 0);
		// 	rl_redisplay();
		// }
	}

}

void	parser_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(STDIN_FILENO, "^C\n", 3);
		if (!DEBUG)
		{
			rl_on_new_line();
			rl_redisplay();
			// rl_replace_line("", 0);
		}
	}
	if (signal == SIGQUIT)
	{
	}
}

void	ft_restore_terminal(int	i)
{
	static struct termios original_term_settings;

	if (!i)
		tcgetattr(STDIN_FILENO, &original_term_settings);
	else
		tcsetattr(STDIN_FILENO, TCSANOW, &original_term_settings);

}
void	ft_configure_terminal(void)
{
	struct termios	new_term;

	tcgetattr(STDIN_FILENO, &new_term);
	new_term.c_lflag &= ~(ECHOCTL);
	new_term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

// void	set_signal_action(void)
// {
// 	struct sigaction	signalaction;

// 	ft_bzero(&signalaction, sizeof(signalaction));
// 	signalaction.sa_handler = signal_handler;
// 	if (sigaction(SIGQUIT, &signalaction, NULL) == -1)
// 		printf("Error: could not set signal action");
// 	if (sigaction(SIGINT, &signalaction, NULL) == -1)
// 		printf("Error: could not set signal action");
// }
