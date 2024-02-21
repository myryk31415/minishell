/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:25:42 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/21 15:16:51 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	input(t_redirect_in *token)
{
	int			j;
	int			file;

	file = 0;
	j = 0;
	while (token->string[j])
	{
		if (file)
			close(file);
		if (token->heredoc[j] == false)
		{
			file = open(token->string[j], O_RDONLY, 0666);
			if (file == -1)
				perror("File Permissions");
		}
		else
			file = here_doc(token->string[j]);
		j++;
	}
	dup2(file, STDIN_FILENO);
	close(file);
}

void	output(t_redirect_out *token)
{
	int		file;
	int		j;

	j = 0;
	file = 0;
	while (token->string[j])
	{
		if (file)
			close(file);
		if (token->append[j] == false)
			file = open(token->string[j],
					O_WRONLY | O_TRUNC | O_CREAT, 0666);
		else
			file = open(token->string[j],
					O_WRONLY | O_APPEND | O_CREAT, 0666);
		if (file == -1)
			perror("File Permissions");
		j++;
	}
	dup2(file, STDOUT_FILENO);
	close(file);
}
