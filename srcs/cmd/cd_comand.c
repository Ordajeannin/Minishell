/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_comand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 11:34:51 by asalic            #+#    #+#             */
/*   Updated: 2023/09/05 10:05:03 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Compte le nombre de directory qui n'existe plus depuis rm -rf.
 * Ignore les /..
*/
int	count_dir(t_shell *shell)
{
	int		len;
	char	*temp;
	DIR		*dir;

	len = 0;
	temp = shell->pwd;
	dir = opendir(temp);
	while (!dir)
	{
		if (temp[ft_strlen(temp) - 1] != '.' && temp[ft_strlen(temp) - 2] \
			!= '.' && temp[ft_strlen(temp) - 3] != '/')
			len ++;
		temp = from_end_to_char(temp, '/');
		closedir(dir);
		dir = opendir(temp);
	}
	closedir(dir);
	return (len -1);
}

/* 
 * Renvoie vers le dernier repertoire existant.
 * Cas ou len_back et len_dir dont egaux.
*/
void	add_back(int len_back, t_args *list)
{
	while (len_back > 0)
	{
		list->next->str = ft_strjoin("../", list->next->str);
		len_back --;
	}
}

/* 
 * Gere le cas ou cd .. et que .. n'existe plus.
 * Dans ce cas la, on compte le nombre de sous-dossiers qui n'existent plus.
 * On compte aussi le nombre de /.. apres les dossiers s'il y en a.
 * On rajoute la difference des deux en /..
 * Si les deux sont egaux alors, revenir au dernier dossier existant.
*/
int	big_problem_cd(t_shell *shell, t_args *list, t_args *env_list)
{
	int		len_dir;
	int		len_back;

	len_dir = count_dir(shell);
	len_back = count_back(shell->pwd);
	if (len_dir > len_back)
	{
		change_env_cd(&env_list, ft_strjoin("OLDPWD=", shell->is_pwd),
			ft_strjoin("OLDPWD=", shell->oldpwd));
		change_env_cd(&env_list, ft_strjoin("PWD=", \
			ft_strjoin(shell->is_pwd, "/..")), ft_strjoin("PWD=", \
			shell->pwd));
		shell->oldpwd = shell->pwd;
		shell->is_pwd = ft_strjoin(shell->is_pwd, "/..");
		shell->pwd = ft_strjoin(shell->is_pwd, "/..");
		shell->error = errno;
		return (1);
	}
	else if (len_dir == len_back)
		add_back(len_back, list);
	return (0);
}

/* 
 * Change de repertoire en fonction du buf envoye.
 * Agit reellement comme la commande cd.
*/
int	cd_real_version(char *buf, t_shell *shell, t_args *env_list, t_args *list)
{
	if (chdir(buf) == -1)
	{
		ft_printf("bash: %s: %s: %s\n", list->str, list->next->str, \
			strerror(errno));
		shell->error = errno;
		return (1);
	}
	else
	{
		change_env_cd(&env_list, ft_strjoin("OLDPWD=", shell->is_pwd),
			ft_strjoin("OLDPWD=", shell->is_oldpwd));
		shell->oldpwd = shell->is_pwd;
		shell->is_oldpwd = shell->is_pwd;
		if (getcwd(NULL, 0) != NULL)
		{
			change_env_cd(&env_list, ft_strjoin("PWD=", getcwd(NULL, 0)),
				ft_strjoin("PWD=", shell->is_pwd));
			shell->is_pwd = getcwd(NULL, 0);
			shell->pwd = getcwd(NULL, 0);
		}
	}
	return (0);
}

/* 
 * Check si cd .. || cd ~ || autre cd
 * Agit en fonction des cas speciaux, un peu comme une gestionnaire
 * d'arguments pour cd specifiquement.
*/
int	ft_cd(t_args *list, t_shell *shell, t_args *env_list)
{
	char	*buf;
	char	*temp;
	DIR		*dir;

	buf = list->next->str;
	temp = NULL;
	if (ft_strncmp(list->next->str, "~", ft_strlen(list->next->str)) == 0)
		buf = ft_strjoin("/home/", shell->user);
	else if (ft_strncmp(list->next->str, "..", ft_strlen(list->next->str)) == 0)
	{
		temp = from_end_to_char(shell->is_pwd, '/');
		dir = opendir(temp);
		if (dir == NULL)
		{
			buf = NULL;
			if (big_problem_cd(shell, list, env_list) == 1)
				return (1);
			buf = list->next->str;
			closedir(dir);
		}
	}
	if (cd_real_version(buf, shell, env_list, list) == 1)
		return (1);
	shell->error = 0;
	return (0);
}
