/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 23:26:49 by padam             #+#    #+#             */
/*   Updated: 2024/03/26 20:08:42 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal;

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		ft_printf("its a me mario");
	}
	if (signal == SIGQUIT)
		ft_printf("its a me luigi");
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
