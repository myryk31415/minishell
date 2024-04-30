/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:25:42 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/30 16:21:55 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	input_permission(char *input)
{
	int	file;

	file = 0;
	file = open(input, O_RDONLY, 0666);
	if (file == -1)
		return (error_msg("minishell: ", input), -1);
	return (file);
}

int	output_permission(char *output, int append)
{
	int	file;

	file = 0;
	if (append == 0)
		file = open(output,
				O_WRONLY | O_TRUNC | O_CREAT, 0666);
	else
		file = open(output,
				O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (!file || file == -1)
		return (error_msg("minishell: ", output), -1);
	return (file);
}

int	handle_both_permission(int *redir, t_cmd *tk, t_exec *exec, int i)
{
	while (tk->redirects[i] || tk->redirect_type[i])
	{
		if (tk->redirect_type[i] == 0 || tk->redirect_type[i] == 1)
		{
			if (redir[1])
				close(redir[1]);
			redir[1] = output_permission(tk->redirects[i], \
				tk->redirect_type[i]);
		}
		else
		{
			if (redir[0])
				close(redir[0]);
			if (tk->redirect_type[i] == 2)
				redir[0] = input_permission(tk->redirects[i]);
			else if (tk->redirect_type[i] > 0)
				redir[0] = tk->redirect_type[i];
			else
				redir[0] = heredoc_expand(-tk->redirect_type[i], exec);
		}
		if (redir[0] == -1 || redir[1] == -1)
			return (-1);
		i++;
	}
	return (0);
}

int	handle_both(int *redir, t_cmd *tk, t_exec *exec)
{
	int		i;

	i = 0;
	redir[0] = 0;
	redir[1] = 0;
	while (tk->redirects[i] || tk->redirect_type[i])
	{
		tk->redirects[i] = expand_variables(tk->redirects[i], exec);
		tk->redirects[i] = expander(tk->redirects[i]);
		i++;
	}
	i = 0;
	if (handle_both_permission(redir, tk, exec, i) == -1)
		return (-1);
	return (0);
}
