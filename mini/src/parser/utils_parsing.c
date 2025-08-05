#include "../../include/minishell.h"

void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->pos < lexer->len && (lexer->input[lexer->pos] == ' '
			|| lexer->input[lexer->pos] == '\t'))
		lexer->pos++;
}
int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ' || c == '\t');
}

void	print_prompt(void)
{
	const char *g_prompt_art = "\033[1;35m\n"
								"\t ███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     \n"
								"\t ████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     \n"
								"\t ██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     \n"
								"\t ██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     \n"
								"\t ██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n"
								"\t ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n"
								"\033[0m\n";
	printf("%s\n", g_prompt_art);
}

int	check_syntax(t_token *tokens)
{
	t_token	*current;
	int		pipe_count;
	int		expect_command;

	current = tokens;
	pipe_count = 0;
	expect_command = 1;
	
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (expect_command)
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				return (0);
			}
			pipe_count++;
			expect_command = 1;
		}
		else if (current->type == TOKEN_WORD)
		{
			expect_command = 0;
		}
		else if (current->type >= TOKEN_REDIRECT_IN && 
				current->type <= TOKEN_HEREDOC)
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				printf("minishell: syntax error near unexpected token\n");
				return (0);
			}
			current = current->next;
		}
		current = current->next;
	}
	
	if (expect_command)
	{
		printf("minishell: syntax error: unexpected end of input\n");
		return (0);
	}
	
	return (1);
}

