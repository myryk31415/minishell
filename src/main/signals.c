/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 23:26:49 by padam             #+#    #+#             */
/*   Updated: 2024/04/25 23:02:12 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal;

void	signal_handler(int signal)
{
	g_signal = 1;
	if (signal == SIGINT)
	{
		write(STDIN_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signal == SIGQUIT)
	{
		write(STDIN_FILENO, "\n", 1);
		ft_putstr_fd("Quit: 3\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}

}

void	set_signal_action(void)
{
	struct sigaction	signalaction;

	ft_bzero(&signalaction, sizeof(signalaction));
	signalaction.sa_handler = signal_handler;
	if (sigaction(SIGQUIT, &signalaction, NULL) == -1)
		printf("Error: could not set signal action");
	if (sigaction(SIGINT, &signalaction, NULL) == -1)
		printf("Error: could not set signal action");
}
