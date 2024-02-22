/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:25:42 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/22 14:58:06 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	input(t_redirect_in *token)
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
				return (perror(error_msg("bash: ", token->string[j])), -1);
		}
		else
			file = here_doc(token->string[j]);
		j++;
	}
	dup2(file, STDIN_FILENO);
	close(file);
	return (0);
}

int	output(t_redirect_out *token)
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
			return (perror(error_msg("bash: ", token->string[j])), -1);
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
