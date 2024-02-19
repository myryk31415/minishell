/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:25:42 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/19 17:48:15 by aweizman         ###   ########.fr       */
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

void	input(t_redirect_in *token, int *fd)
{
	int			j;

	j = 0;
	while (token->string[j])
	{
		if (token->heredoc[j] == false)
			input_files(token->string[j], fd);
		else
			here_doc(token->string[j], fd);
		j++;
	}
	close(fd[1]);
}

char	*output_string(int	*fd)
{
	char	*output;
	char	*str;
	char	*tmp;

	output = malloc(sizeof(char));
	str = NULL;
	tmp = NULL;
	while (1)
	{
		str = get_next_line(fd[0]);
		if (str && *str)
		{
			tmp = ft_strjoin(output, str);
			free(output);
			free(str);
			output = tmp;
		}
		return (output);
	}
}

void	output_add(t_redirect_out *token, char *output_str, int j)
{
	int		file;
	int		id;

	id = 0;
	file = 0;
	if (token->string[j])
	{
		id = fork();
		if (id != 0)
			output_add(token, output_str, j + 1);
		else
		{
			if (token->append[j] == false)
				file = open(token->string[j],
						O_WRONLY | O_TRUNC | O_CREAT, 0666);
			else
				file = open(token->string[j],
						O_WRONLY | O_APPEND | O_CREAT, 0666);
			write(file, output_str, ft_strlen(output_str));
			// dup2(file, STDOUT_FILENO);
			close(file);
			// exec(token->cmd);
		}
	}
}

void	output(t_redirect_out *token, int *fd)
{
	char	*output_str;

	output_str = output_string(fd);
	ft_printf("%s", output_str);
	output_add(token, output_str, 0);
}
