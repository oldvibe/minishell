#include "../../include/parser.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->heredoc_delimiter = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	add_arg_to_cmd(t_cmd *cmd, char *arg)
{
	int	count;

	count = 0;
	if (cmd->args)
	{
		while (cmd->args[count])
			count++;
	}
	cmd->args = realloc(cmd->args, sizeof(char *) * (count + 2));
	if (!cmd->args)
		return (0); 

	cmd->args[count] = strdup(arg);
	cmd->args[count + 1] = NULL;    // NULL terminator
	return (1);
}

// int main()
// {
//     t_cmd *cmd = init_cmd();
//     if (!cmd)
//     {
//         printf("Failed to create command\n");
//         return (1);
//     }
    
//     printf("=== Testing add_arg_to_cmd ===\n");
    
//     add_arg_to_cmd(cmd, "ls");
//     add_arg_to_cmd(cmd, "-la");
//     add_arg_to_cmd(cmd, "/home");
    
//     // // Print arguments
//     printf("Command arguments:\n");
//     if (cmd->args)
//     {
//         int i = 0;
//         while (cmd->args[i])
//         {
//             printf("  args[%d] = '%s'\n", i, cmd->args[i]);
//             i++;
//         }
//     }
    
//     if (cmd->args)
//     {
//         for (int i = 0; cmd->args[i]; i++)
//             free(cmd->args[i]);
//         free(cmd->args);
//     }
//     free(cmd);
    
//     return (0);
// }
