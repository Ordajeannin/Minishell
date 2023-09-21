/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:37:52 by asalic            #+#    #+#             */
/*   Updated: 2023/09/21 18:00:06 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Cherche s2 dans s1.
 * Parcours s1 jusqu'a trouver s2 a la suite.
*/
int	find_opt(char *s1, char *s2)
{
	size_t	i;

	i = 1;
	if (s1[0] != '-')
		return (0);
	while (s1[i] && s1[i] == 'n')
	{
		i ++;
		if (!s1[i])
			return (1);
	}
	return (0);
}

/* 
 * Affiche caractere par caractere la liste.
 * Suite de ft_echo.
 * // if (list->token == TOKEN_TEMP_VAR)
	// 	process_not_s_quotes(list, env_list, 2);
*/
static void	iter_echo(t_args *list, t_args **env_list)
{
	int		i;
	char	*save;

	i = 0;
	save = list->str;
	if (list->str == NULL)
		return ;
	while (list->str[i])
	{
		write (1, &list->str[i], 1);
		i ++;
	}
}

void	echo_loop(t_args *list, t_args **env_list)
{
	while (list && list->token != TOKEN_AND && list->token != TOKEN_OR)
	{
		iter_echo(list, env_list);
		list = list->next;
		if (list != NULL)
			write (1, " ", 1);
	}
}

/*
 * Fonction a l'image de 'echo'.
 * Affiche caractere par caractere les arguments en ignorant les quotes.
 * (Attention: cas particuliers, quotes a l'interieur de d'autres).
*/
int	ft_echo(t_args *list, t_shell *shell, t_args **env_list)
{
	int	bools;

	bools = 0;
	if (list == NULL)
	{
		write (1, "\n", 1);
		return (1);
	}
	while (list && ft_strcmp(list->str, "\t") == 0)
		list = list->next;
	while (list && find_opt(list->str, "-n") == 1)
	{
		list = list->next;
		bools = 1;
	}
	echo_loop(list, env_list);
	if (bools == 0)
		write (1, "\n", 1);
	change_error(env_list, 0);
	return (0);
}
