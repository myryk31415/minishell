/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 23:10:31 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 18:39:57 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node_type	err_pars(char *message, t_cmd *redirects, t_token **tokens)
{
	char	*error;

	cmd_free(redirects);
	error = ft_strjoin("minishell: ", message);
	perror(error);
	free(error);
	token_delete_all(tokens);
	return (ERROR);
}

void	fill_list(t_token *token, char **type_list)
{
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

void	print_syntax_err(t_token *token)
{
	char	*type_list[11];

	type_list[0] = "word";
	type_list[1] = "separator";
	type_list[2] = "|";
	type_list[3] = "&&";
	type_list[4] = "||";
	type_list[5] = "(";
	type_list[6] = ")";
	type_list[7] = "<";
	type_list[8] = ">";
	type_list[9] = ">>";
	type_list[10] = "<<";
	fill_list(token, type_list);
}
