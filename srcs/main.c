/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 20:36:50 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/04 09:51:25 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet a main d'etre a moins de 25 lines
 * Fonction purement utilitaire, ne pas garder dans le rendu final
*/
static void	ft_gain_place(char **av, t_args **list, char **input,
		t_args **env_list)
{
	*list = NULL;
	*input = NULL;
	*env_list = NULL;
	(void)av;
}

/* 
 * Gestionnaire de signaux
 * Renvoie l'invite de commande lorsque sig == CTRL-C
 */
void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	return ;
}

/* 
 * Suite du main.
*/
static void	main_bis(char *input, t_args *list, t_args *env_list, \
	t_shell *shell)
{
	add_history(input);
	from_input_to_list_of_args(input, &list, &env_list);
	if (list)
	{
		print_args_list(&list);
		args_handle(list, shell, &env_list, input);
	}
	free(input);
	clear_args_list(&list);
}

/*
 * Actions de la boucle ATM
 * 1) readline ("prompt")
 * 2) splitage de l'input en tableau de chaine
 * 3) analyse de chaque chaine, attribution des tokens
 * 4) affichage de l'analyse (temporaire)
 * 5) gestionnaire de commandes
 * 6) gere l'historique de commande
 * 7) free le precedent input avant de reproposer le prompt
 * 8) vide la liste d'arguments, mais conserve le pointeur
*/
int	main(int ac, char **av, char **env)
{
	char	*input;
	t_args	*list;
	t_args	*env_list;
	char	*user;
	t_shell	shell;

	(void)ac;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	ft_gain_place(av, &list, &input, &env_list);
	if (set_env(&env_list, env, &shell) == -1)
		return (-1);
	user = shell.user;
	while (1)
	{
		input = readline(prompt_cmd(&shell, user));
		if (input == NULL)
			ft_exit(input, list, env_list, &shell);
		main_bis(input, list, env_list, &shell);
	}
	return (0);
}
