/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 23:10:31 by padam             #+#    #+#             */
/*   Updated: 2024/04/07 18:32:08 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node_type	err_pars(char *message, t_cmd *redirects, t_token **tokens)
{
	char *error;

	cmd_free(redirects);
	error = ft_strjoin("minishell: ", message);
	perror(error);
	free(error);
	token_delete_all(tokens);
	return (ERROR);
}

void	print_syntax_err(t_token *token)
{
	char	*type_list[] = {"word", "separator", "|", "&&", "||", "(", ")",
				 "<", ">", ">>", "<<"};
	if (!token)
		ft_putstr_fd("minishell: syntax error near missing tokens\n", 2);
	else if (token->value)
	{

		ft_putstr_fd("minishell: syntax error near unexpected token `\n", 2);
		ft_putstr_fd(token->value, 2);
		ft_putstr_fd("'\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(type_list[token->type], 2);
		ft_putstr_fd("'\n", 2);
	}
	token_delete_all(&token);
}
