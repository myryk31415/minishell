/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:25:42 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/29 15:37:51 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	input(char **input, bool *heredoc)
{
	int			j;
	int			file;

	file = 0;
	j = 0;
	while (input[j])
	{
		if (file)
			close(file);
		if (heredoc[j] == false)
		{
			file = open(input[j], O_RDONLY, 0666);
			if (file == -1)
				return (perror(error_msg("bash: ", input[j])), -1);
		}
		else
			file = here_doc(input[j]);
		j++;
	}
	dup2(file, STDIN_FILENO);
	close(file);
	return (0);
}

int	output(char **output, bool *append)
{
	int		file;
	int		j;

	j = 0;
	file = 0;
	while (output[j])
	{
		if (file)
			close(file);
		if (append[j] == false)
			file = open(output[j],
					O_WRONLY | O_TRUNC | O_CREAT, 0666);
		else
			file = open(output[j],
					O_WRONLY | O_APPEND | O_CREAT, 0666);
		if (file == -1)
			return (perror(error_msg("bash: ", output[j])), -1);
		j++;
	}
	dup2(file, STDOUT_FILENO);
	close(file);
	return (0);
}

char	*error_msg(char *cmd, char *file)
{
	char	*tmp;

	tmp = ft_strjoin(cmd, file);
	return (tmp);
}
