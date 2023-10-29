/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoferre <leoferre@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 18:02:28 by macarval          #+#    #+#             */
/*   Updated: 2023/10/29 13:16:09 by leoferre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/token.h"
#include "../headers/parser.h"
#include "../headers/execute.h"
#include "../headers/signals.h"
#include "../headers/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_token		*token_head;
	t_cmd_table	*cmd_table;
	t_shell		shell;
	
	argv++;
	token_head = NULL;
	cmd_table = init_cmd_table(&token_head, envp);
	if (argc != 1)
		return (printf("can't accept multiple arguments\n"), 1);
	while (1)
	{
		sign_init();
		shell.line = get_line_text(cmd_table->latest_exit_code);
		if (!shell.line)
			continue ;
		cmd_table->input_string = shell.line;
		//if (shell.line[0] != '\0')
		//{
			if (!tokenize_string(shell.line, cmd_table))
				exec_minishell(cmd_table, token_head);
			//else
				//free(shell.line);
			free_func_token(&token_head);
		//}
		//else
			//free(shell.line);
	}
	return (0);
}

void	exec_minishell(t_cmd_table *cmd_table, t_token *token_head)
{
	fill_cmd_table(cmd_table, &token_head);
	execute(cmd_table);
	free_func_cmd_table(cmd_table);
}

char	*get_line_text(int latest_exit_code)
{
	//char		*text;
	static char	*line;
	
	if (line)
	{
		free(line);
		line = NULL;
	}
	//text = make_text();
	line = strtrim_mod(readline("minishell$ "), " ");
	if (!line)
		ctrl_d(latest_exit_code);
	if (line[0] == '\0')
		return (NULL);
	//free(text);
	add_history(line);
	return (line);
}
