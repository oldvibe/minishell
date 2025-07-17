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