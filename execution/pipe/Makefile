# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/08 16:57:44 by aweizman          #+#    #+#              #
#    Updated: 2024/02/14 16:54:55 by aweizman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= pipex

NAME_BONUS	= pipex_bonus

LIBFT		= ../../libft

CC			= cc

CFLAGS		= -Wall -Wextra -Werror

HEADERS		= -I./pipex.h -I$(LIBFT)/include/libft.h

SRCS		= main utils fork

SRCS_BONUS 	= main_bonus utils_bonus fork_bonus

OBJ_DIR		= obj/
SRC			= 	$(addsuffix .c, $(SRCS))
OBJ 		= 	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRCS)))
SRC_BONUS	= 	$(addsuffix .c, $(SRCS_BONUS))
OBJ_BONUS 	= 	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRCS_BONUS)))
OBJ_CHECK	=	.cache_exists


all : $(NAME)

$(NAME): $(SRC) $(LIBFT)/libftprintf.a $(OBJ)
	@echo "compiling pipex.."
	@$(CC) $(CFLAGS) -o $(NAME) $(SRC) $(LIBFT)/libftprintf.a $(HEADERS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_CHECK)
		@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

$(LIBFT)/libftprintf.a:
	@make -C $(LIBFT)

$(OBJ_CHECK):
		@mkdir -p $(OBJ_DIR)
clean:
	@echo "cleaning up..."
	@rm -fr $(OBJ_DIR)
	@make clean -C $(LIBFT)

fclean: clean
	@echo "full cleaning up..."
	@make fclean -C $(LIBFT)
	@rm -f $(NAME)
	@rm -f $(NAME_BONUS)

bonus:	$(SRC_BONUS) $(LIBFT)/libftprintf.a $(OBJ_BONUS)
	@echo "compiling pipex bonus.."
	@$(CC) $(CFLAGS) -o $(NAME_BONUS) $(SRC_BONUS) $(LIBFT)/libftprintf.a $(HEADERS)

re: fclean all

.PHONY: all clean fclean re

