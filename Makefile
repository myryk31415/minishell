# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/09 15:34:56 by padam             #+#    #+#              #
#    Updated: 2024/04/30 16:35:50 by antonweizma      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS =  $(INCLUDES) -g3 -Wall -Wextra -Werror
INCLUDES = -I./includes
LIBS = -lreadline
LIBFT_DIR = libft

SRC_PATH = src
OBJ_PATH = obj

OBJ_DIRS =	execution/nodes	execution/builtins	execution/utils	main	main/expansion \
			parser	parser/binary_tree	\

SRCS_MAIN =	main.c cleanup.c signals.c

SRCS_EXP =	variable_expansion.c expander.c tilde_expansion.c

SRCS_PARS =	is_checks.c			parser.c			tokenizer.c		\
			tokens_utils.c			debug.c				cleanup.c	\
			check_unclosed.c

SRCS_BNTR =	node_struct.c			node_utils.c		pipeline.c	\
			redirects.c				heredoc.c

SRCS_BUILT =	cd.c	echo_env_pwd.c		export.c	unset.c		\
				exit.c	builtins.c

SRCS_NODE =		pipe.c		redirect.c		and_or.c command.c

SRCS_UTILS_EXEC =	closing.c		error.c		execute.c		handle_both.c	\
					input_output.c		utils.c


SRC_NAME =	$(addprefix main/,						$(SRCS_MAIN))		\
			$(addprefix main/expansion/,			$(SRCS_EXP))		\
			$(addprefix execution/builtins/,		$(SRCS_BUILT))		\
			$(addprefix execution/nodes/,			$(SRCS_NODE))		\
			$(addprefix execution/utils/,			$(SRCS_UTILS_EXEC))	\
			$(addprefix parser/,					$(SRCS_PARS))		\
			$(addprefix parser/binary_tree/,		$(SRCS_BNTR))		\

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
RESET = \033[0m

OBJ_NAME = $(SRC_NAME:.c=.o)
OBJS = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

all: $(NAME)

$(NAME): ascii_art $(LIBFT_DIR)/libft.a $(OBJS)
	@$(CC) -o $(NAME) $(CFLAGS) $(LIBS) $(OBJS) $(LIBFT_DIR)/libft.a
	@printf "%-100s\n" "$(NAME) compiled"

$(LIBFT_DIR)/libft.a:
	@make -C $(LIBFT_DIR)

$(OBJ_PATH)	:
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(addprefix $(OBJ_PATH)/,$(OBJ_DIRS))

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	@printf "%-100s\r" "$(CC) $(CFLAGS) -o $@"
	@$(CC) $(CFLAGS) -o $@ -c $<

init_submodules:
	git submodule update --init --recursive

update_submodules:
	git submodule update --recursive --remote

clean:
	@rm -rf $(OBJ_PATH)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

ascii_art:
# 	@clear
# 	@printf "$(GREEN)	                                                                                                                                         \n\
# 	                                                                                                                                         \n\
#                                                                                                                                              \n\
# MMMMMMMM               MMMMMMMM  iiii                      iiii     SSSSSSSSSSSSSSS hhhhhhh                                 lllllll lllllll  \n\
# M:::::::M             M:::::::M i::::i                    i::::i  SS:::::::::::::::Sh:::::h                                 l:::::l l:::::l  \n\
# M::::::::M           M::::::::M  iiii                      iiii  S:::::SSSSSS::::::Sh:::::h                                 l:::::l l:::::l  \n\
# M:::::::::M         M:::::::::M                                  S:::::S     SSSSSSSh:::::h                                 l:::::l l:::::l  \n\
# M::::::::::M       M::::::::::Miiiiiii nnnn  nnnnnnnn    iiiiiii S:::::S             h::::h hhhhh           eeeeeeeeeeee     l::::l  l::::l  \n\
# M:::::::::::M     M:::::::::::Mi:::::i n:::nn::::::::nn  i:::::i S:::::S             h::::hh:::::hhh      ee::::::::::::ee   l::::l  l::::l  \n\
# M:::::::M::::M   M::::M:::::::M i::::i n::::::::::::::nn  i::::i  S::::SSSS          h::::::::::::::hh   e::::::eeeee:::::ee l::::l  l::::l  \n\
# M::::::M M::::M M::::M M::::::M i::::i nn:::::::::::::::n i::::i   SS::::::SSSSS     h:::::::hhh::::::h e::::::e     e:::::e l::::l  l::::l  \n\
# M::::::M  M::::M::::M  M::::::M i::::i   n:::::nnnn:::::n i::::i     SSS::::::::SS   h::::::h   h::::::he:::::::eeeee::::::e l::::l  l::::l  \n\
# M::::::M   M:::::::M   M::::::M i::::i   n::::n    n::::n i::::i        SSSSSS::::S  h:::::h     h:::::he:::::::::::::::::e  l::::l  l::::l  \n\
# M::::::M    M:::::M    M::::::M i::::i   n::::n    n::::n i::::i             S:::::S h:::::h     h:::::he::::::eeeeeeeeeee   l::::l  l::::l  \n\
# M::::::M     MMMMM     M::::::M i::::i   n::::n    n::::n i::::i             S:::::S h:::::h     h:::::he:::::::e            l::::l  l::::l  \n\
# M::::::M               M::::::Mi::::::i  n::::n    n::::ni::::::iSSSSSSS     S:::::S h:::::h     h:::::he::::::::e          l::::::ll::::::l \n\
# M::::::M               M::::::Mi::::::i  n::::n    n::::ni::::::iS::::::SSSSSS:::::S h:::::h     h:::::h e::::::::eeeeeeee  l::::::ll::::::l \n\
# M::::::M               M::::::Mi::::::i  n::::n    n::::ni::::::iS:::::::::::::::SS  h:::::h     h:::::h  ee:::::::::::::e  l::::::ll::::::l \n\
# MMMMMMMM               MMMMMMMMiiiiiiii  nnnnnn    nnnnnniiiiiiii SSSSSSSSSSSSSSS    hhhhhhh     hhhhhhh    eeeeeeeeeeeeee  llllllllllllllll \n\
#                                                                                                                                              \n\
#                                                                                                                      $(BLUE)by: padam && aweizman$(RESET) \n\
#                                                                                                                                              \n"

.PHONY: all, $(NAME), $(LIBFT_DIR)/libft.a, $(OBJ_PATH), $(OBJ_PATH), clean, fclean, re
