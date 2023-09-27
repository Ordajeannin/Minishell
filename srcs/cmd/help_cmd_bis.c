/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_cmd_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 08:42:25 by asalic            #+#    #+#             */
/*   Updated: 2023/09/27 11:58:04 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Fonction d'extension de cd.
 * Permets de mettre a jour ou d'export pwd et oldpwd lorsque cd change
 * de repertoire.
*/
int	cd_move_and_change(t_args *env_list, t_shell *shell)
{
	char	*old_pwd_change;
	char	*old_cmd;
	char	*current_cmd;
	char	*new_pwd;

	old_pwd_change = ft_strjoin("OLDPWD=", shell->is_pwd);
	if (!old_pwd_change)
		return (1);
	old_cmd = ft_strjoin("OLDPWD=", shell->is_oldpwd);
	if (!old_cmd)
	{
		free(old_pwd_change);
		return (1);
	}
	change_env_cd(&env_list, old_pwd_change, old_cmd);
	free(old_cmd);
	free(old_pwd_change);
	shell->is_oldpwd = shell->is_pwd;
	shell->oldpwd = shell->is_pwd;
	current_cmd = getcwd(NULL, 0);
	if (current_cmd != NULL)
	{
		new_pwd = ft_strjoin("PWD=", current_cmd);
		if (!new_pwd)
			return (1);
		old_cmd = ft_strjoin("PWD=", shell->is_pwd);
		if (!old_cmd)
		{
			free(new_pwd);
			return (1);
		}
		change_env_cd(&env_list, new_pwd, old_cmd);
		shell->is_pwd = current_cmd;
		shell->pwd = current_cmd;
		free(new_pwd);
		free(old_cmd);
	}
	else
		return (1);
	return (0);
}

/* 
 * Update la VE $_
 * Cette VE prends comme valeur le dernier argument passe en input
 * Mis a jour a chaque tour de boucle prompt
 * Exception pour env
*/
int	update_last_ve(t_args **list, t_args **env_list)
{
	t_args	*current;

	current = *list;
	if (ft_strcmp("env", current->str) == 0 && ft_strlen(current->str) == 3)
	{
		if (change_env_exp(env_list, "_", "/usr/bin/env") == 2)
			return (1);
		return (0);
	}
	while (current && current->next != NULL)
	{
		current = current->next;
		if (ft_strcmp(current->str, "\0") == 0)
			break ;
	}
	if (change_env_exp(env_list, "_", current->str) == 2)
		return (1);
	return (0);
}
