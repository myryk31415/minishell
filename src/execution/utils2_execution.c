/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:57:38 by antonweizma       #+#    #+#             */
/*   Updated: 2024/03/25 20:33:06 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	close_pipes(int **pipes)
{
	if (pipes && pipes[1])
	{
		close(pipes[1][0]);
		close(pipes[1][1]);
	}
	if (pipes && pipes[0])
	{
		close(pipes[0][0]);
		close(pipes[0][1]);
	}

}

void	close_pipe(int *pipe)
{
	if (pipe)
	{
		close(pipe[0]);
		close(pipe[1]);
	}
}
