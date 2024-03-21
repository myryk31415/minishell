/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:57:38 by antonweizma       #+#    #+#             */
/*   Updated: 2024/03/21 14:58:05 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	close_pipes(int *fd, int *pre_fd)
{
	if (fd)
	{
		close(fd[0]);
		close(fd[1]);
	}
	if (pre_fd)
	{
		close(pre_fd[0]);
		close(pre_fd[1]);
	}

}
