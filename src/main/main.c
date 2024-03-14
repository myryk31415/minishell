/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/03/14 20:12:10 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	main(void)
{
	voidhhhh		*tree;
	t_cmd		*test;
	t_node_type	type;

	tree = NULL;
	while (1)
	{
		type = parser(&tree);
		test = tree;
		execution(tree, type);

	}
	free (tree);
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
