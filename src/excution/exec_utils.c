#include "../../include/minishell.h"

int is_built(char **args)
{
    if (!args || !args[0])
		return (0);
	if (ft_strcmp(args[0], "exit") == 0)
        return (1);
	if (ft_strcmp(args[0], "pwd") == 0)
	    return (1);
	if (ft_strcmp(args[0], "cd") == 0)
        return (1);
	if (ft_strcmp(args[0], "echo") == 0)
        return (1);
	if (ft_strcmp(args[0], "env") == 0)
        return (1);
	if (ft_strcmp(args[0], "export") == 0)
        return (1);
	return (0);
}


void	handle_redir(t_cmd *cmd, redire_status status, int tmpin, int tmpout)
{
	int f_out = -2;
	int f_in = -2;

	if(status == OPEN_RED)
	{
		while (cmd->red)
		{
			if(cmd->red-> == '>')
			{
				while (cmd->red>next && (cmd->red->next-> == '>' || cmd->red->next-> == '>>' ))
				{
					if(cmd->red->next-> == '>')
					{
						f_out = open(cmd->red->file_name, O_CREAT, O_WRONLY);
						if(f_out == -1)
						{
							perror("cmd->red->filename");
							exit(1);
						}
						close(f_out);

					}
					else if(cmd->red->next-> == '>>')
					{
						f_out = open(cmd->red->file_name, O_CREAT, O_WRONLY, O_APPEND);
						{
							perror("cmd->red->filename");
							exit(1);
						}
						close(f_out);
					}
					cmd->red = cmd->red->next;
				}
				if(cmd->red->next-> == '>')
				{
					f_out = open(cmd->red->file_name, O_CREAT, O_WRONLY);
					dup2(f_out, 1);
				}
				else if(cmd->red->next-> == '>>')
				{
					f_out = open(cmd->red->file_name, O_CREAT, O_WRONLY);
					dup2(f_out, 1);
				}
	
			}	
			}
			if(cmd->red-> == '<')
			{
				while (cmd->red>next && cmd->red->next-> == '<')
				{
					f_in = open(cmd->red->file_name, O_RDONLY);
					if(f_in == -1)
					{
						perror("cmd->red->filename");
						exit(1);
					}
					close(f_in);
					cmd->red = cmd->red->next;
				}
				f_in = open(cmd->red->file_name, O_CREAT, O_WRONLY);
				dup2(f_in, 1);
				
	
			}
			cmd->red = cmd->red->next;
	
		}

	}
	else
	{
		if(f_in != -2)
		{
			dup2(tmpin, 0);
			close(tmpin);
			close(f_in);

		}
		if(f_out != -2)
		{
			dup2(tmpin, 1);
			close(tmpin);
			close(f_out);
		

		}
	}
	
}
