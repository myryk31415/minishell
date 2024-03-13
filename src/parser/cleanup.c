/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 23:10:31 by padam             #+#    #+#             */
/*   Updated: 2024/03/12 17:13:50 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void print_err(char *message)
{
	char *error;

	error = ft_strjoin("minishell: ", message);
	perror(error);
	free(error);
}
t_node_type	err_pars(char *message, t_cmd *redirects, t_token *tokens)
{
	free(redirects);
	token_delete_all(&tokens);
	print_err(message);
	return (ERROR);
}

void	print_syntax_err(t_token *token)
{
	char	*type_list[] = {"word", "separator", "|", "&&", "||", "(", ")",
				 "<", ">", ">>", "<<"};
	if (!token)
		printf("minishell: syntax error near missing tokens\n") ;
	else if (token->value)
		printf("minishell: syntax error near unexpected token `%s'\n",
			token->value);
	else
		printf("minishell: syntax error near unexpected token `%s'\n",
			type_list[token->type]);
}
