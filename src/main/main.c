/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/03/08 16:02:23 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

// int	main(void)
// {
// 	void	*tree;

// 	tree = NULL;
// 		if (parser(tree) == AND)
// 		{

// 			and_execute(tree, NULL, NULL, 0);
// 		}
// }

int	main(int argc, char **argv)
{
	int	pid;
	int	status;

	argc=0;
	argv++;
	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (!pid)
		exec(argv);
	else
	{
		waitpid(pid, &status, 0);
		ft_printf("exit: %i\n", status);
		if (status == 256)
			ft_printf("ERROR");
		if (!status)
			ft_printf("SUCCESS");

	}
}
