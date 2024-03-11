/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/03/11 12:20:38 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	main(void)
{
	void	*tree;
	t_node	*test;

	tree = NULL;
		if (parser(tree) == AND)
		{
			test = tree;
			and_execute(tree, NULL, NULL, 0);
		}
	(void)test;
}

// int	main()
// {
// 	int	pid;
// 	int	status;

// 	argc=0;
// 	argv++;
// 	pid = fork();
// 	if (pid == -1)
// 		perror("Fork");
// 	if (!pid)
// 		exec(argv);
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 		ft_printf("exit: %i\n", status);
// 		if (status == 256)
// 			ft_printf("ERROR");
// 		if (!status)
// 			ft_printf("SUCCESS");

// 	}
// }
