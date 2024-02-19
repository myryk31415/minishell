/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:25:42 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/19 15:28:52 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	input_files(char *file_str, int *fd)
{
	char	*str;
	int		file;

	file = open(file_str, O_RDONLY, 0666);
	if (file == -1)
	{
		perror("Infile");
	}
	str = NULL;
	while (1)
	{
		str = get_next_line(file);
		if (str && *str)
		{
			write(fd[1], str, ft_strlen(str));
			free(str);
		}
		else
			break ;
	}
	close(file);
}

void	input(t_redirect_in *token, int *fd, int i)
{
	int			j;

	j = 0;
	if (i == 1)
	{
		while (token->string[j])
		{
			if (token->heredoc[j] == false)
				input_files(token->string[j], fd);
			else
				here_doc(token->string[j], fd);
			j++;
		}
	}
	close(fd[1]);
}
