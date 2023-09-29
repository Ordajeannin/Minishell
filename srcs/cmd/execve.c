/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 17:18:10 by asalic            #+#    #+#             */
/*   Updated: 2023/09/29 10:44:47 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Change env pour $?.
 * Mets a jour les cas d'erreurs de $?
*/
int	change_error(t_args **env_list, int value)
{
	t_args	*current;
	char	*current_name;
	char	*result;
	char 	*nb_char;

	nb_char = ft_itoa(value);
	if (!nb_char)
		return (2);
	result = ft_strjoin("?=", nb_char);
	if (!result)
	{
		free(nb_char);
		return (2);
	}
	current = *env_list;
	while (current)
	{
		current_name = ft_strdupto_n(current->str, '=');
		if (!current_name)
		{
			free(nb_char);
			free(result);
			return (2);
		}
		if (ft_strcmp(current_name, "?") == 0
			&& ft_strlen(current_name) == 1)
		{
			current->str = result;
			free(current_name);
			free(nb_char);
			return (1);
		}
		current = current->next;
	}
	free(nb_char);
	free(current_name);
	free(result);
	return (0);
}

/* 
 * Debut de all_cmd.
 * Cherche si la commande existe ou non a partir de PATH.
 * Exception: si la commande est env, redirection vers builtins.
*/
static char	*error_cmd(t_args *arg, t_shell *shell, t_args *list,
	t_args **env_list)
{
	char	*command;

	command = is_path_or_cmd(shell->cmd_paths, arg->str, shell, env_list);
	if (command == NULL)
	{
		if (access(list->str, F_OK) == 0)
			return (list->str);
		return (NULL);
	}
	if (ft_strncmp(command, "It's env", ft_strlen(command)) == 0)
		ft_env(list, env_list);
	return (command);
}

/* 
 * Preparation du bios avant la creation du processus enfant.
 * Check si la commande est valide ou que son path est valide.
 * Mets tous les arguments dans un shell->input (char **).
 * CTRL-\ actif pour les commandes bloquantes comme cat
*/
static char	*bfore_execution(t_args *arg, t_shell *shell, t_args **list,
	t_args **env_list)
{
	char	*command;

	command = error_cmd(arg, shell, *list, env_list);
	if (command == NULL)
		return (NULL);
	if (ft_strncmp(command, "It's env", ft_strlen(command)) == 0)
		return (NULL);
	if (loop_args(shell, list) == 1)
		return (NULL);
	if (ft_strcmp("./minishell", command) != 0)
	{
		signal(SIGQUIT, signal_handler);
		g_error = 2;
	}
	return (command);
}

/*
 * Processus parents en attente
 * Attends la fin du processus enfant et renvoie une erreur si
 * le processus enfant en a detecte une.
 * Exception: pour ./minishell, -1 a la VE de SHLVL
*/
static int	next_execution(pid_t pid_child, t_args **env_list)
{
	int		status;

	waitpid(pid_child, &status, 0);
	signal(SIGQUIT, SIG_IGN);
	if (g_error == 2)
		g_error = 0;
	if (WEXITSTATUS(status) != 0)
	{
		errno = WEXITSTATUS(status);
		change_error(env_list, handle_error(errno));
		return (1);
	}
	else if (WTERMSIG(status) == SIGSEGV)
	{
		ft_printf("Segmentation Fault (core dumped)\n");
		change_error(env_list, 139);
		return (1);
	}
	return (0);
}

/* 
 * Execution des commandes dependantes de PATH.
 * Creation d'un sous-processus pour execve.
 * Check si la commande est valide.
 * Check si ses arguments sont valides.
 * Si non, return (1) et arret de l'execution de l'invite de commandes.
*/
int	all_cmd(t_args *arg, t_shell *shell, t_args **list, t_args **env_list)
{
	pid_t	pid_child;
	char	*command;
	char	**env_tab;

	env_tab = NULL;
	command = bfore_execution(arg, shell, list, env_list);
	if (command == NULL)
		return (1);
	pid_child = fork();
	if (pid_child == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid_child == 0)
	{
		env_tab = dup_double_string(env_list);
		if (!env_tab)
		{
			free(command);
			return (1);
		}
		execve(command, shell->input, env_tab);
		ft_printf("%s : %s\n", command, strerror(errno));
		exit(handle_error(errno));
	}
	if (env_tab)
	{
		while (*env_tab != NULL)
		{
			free(*env_tab);
			(*env_tab) ++;
		}
	}
	free(command);
	if (next_execution(pid_child, env_list) == 1 || !change_error(env_list, 0))
		return (1);
	return (0);
}
