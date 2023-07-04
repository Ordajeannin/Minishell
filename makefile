# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/03 20:32:59 by ajeannin          #+#    #+#              #
#    Updated: 2023/07/04 12:20:22 by ajeannin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

########## V A R I A B L E S ###########

NAME			= minishell
INCLUDES		= -Iincludes -Ilibft
LIBFT			= -Llibft -lft
SRC_DIR			= srcs/
OBJ_DIR			= obj/
CC				= gcc
CFLAGS			= -Wall -Werror -Wextra
READLINE		= -lreadline


############# S O U R C E S ############

SRC_FILES		= main	\
				  
SRC				= $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJ				= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))



################ R U L E S #############

all				:		$(NAME)

$(NAME)			:		$(OBJ)
					@make -s -C libft/
					@$(CC) $(OBJ) $(INCLUDES) $(LIBFT) -o $(NAME) $(READLINE)
					@echo "Compilation Minishell: done"
					@cd srcs && ctags -R
					@echo "tags are available"

$(OBJ_DIR)%.o 	:		$(SRC_DIR)%.c
					@mkdir -p $(OBJ_DIR)
					@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean			:
					@make clean -s -C libft/
					@rm -rf $(OBJ_DIR)
					@echo "Clean obj_dir: done"

fclean			:		clean
					@make fclean -s -C libft/
					@rm -rf $(NAME)
					@echo "Fclean minishell: done"

re 				: 		fclean all

norme			:
					@echo "\n\nNorme Includes\n"
					@norminette includes
					@echo "\n\nNorme Sources\n"
					@norminette srcs

