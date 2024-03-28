/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 23:10:31 by padam             #+#    #+#             */
/*   Updated: 2024/03/28 13:15:52 by padam            ###   ########.fr       */
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
		printf("minishell: syntax error near missing tokens\n") ;
	else if (token->value)
		printf("minishell: syntax error near unexpected token `%s'\n",
			token->value);
	else
		printf("minishell: syntax error near unexpected token `%s'\n",
			type_list[token->type]);
	token_delete_all(&token);
}
