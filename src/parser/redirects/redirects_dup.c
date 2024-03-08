/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_dup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 23:10:31 by padam             #+#    #+#             */
/*   Updated: 2024/03/08 11:43:28 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_str_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	cmd_free(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_str_array(cmd->args);
	free_str_array(cmd->redirect_in);
	free_str_array(cmd->redirect_out);
	free(cmd->heredoc);
	free(cmd->append);
	free(cmd);
}

t_node_type	err_pars(char *message, t_cmd *redirects, t_token *tokens)
{
	cmd_free(redirects);
	token_delete_all(&tokens);
	printf("error: %s\n", message);
	return (ERROR);
}
