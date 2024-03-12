# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/09 15:34:56 by padam             #+#    #+#              #
#    Updated: 2024/03/12 08:40:29 by antonweizma      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES) -g3
INCLUDES = -I./includes
LIBS = -lreadline
LIBFT_DIR = libft

SRC_PATH = src
OBJ_PATH = obj

OBJ_DIRS =	execution	execution/pipe	main	parser	parser/binary_tree	\
			parser/redirects

SRCS_MAIN =	main.c

SRCS_PARS =	parser_utils.c			parser.c			tokenizer.c	\
			tokens_utils.c			redirects.c			cleanup.c\

SRCS_BNTR =	node_struct.c			node_utils.c		pipeline.c

SRCS_REDIR =	redirects.c			redirects_dup.c

SRCS_EXECUTION =	execution.c utils_execution.c input_output.c and.c

SRCS_BUILTINS = echo.c		cd.c

SRCS_PIPE =			fork.c			pipe.c			utils.c

SRC_NAME =	$(addprefix main/,				$(SRCS_MAIN))		\
			$(addprefix execution/,			$(SRCS_EXECUTION))	\
			$(addprefix parser/,			$(SRCS_PARS))		\
			$(addprefix parser/binary_tree/,$(SRCS_BNTR))		\
			$(addprefix parser/redirects/,	$(SRCS_REDIR))		\
#			$(addprefix execution/pipe/,	$(SRCS_PIPE))		\

OBJ_NAME = $(SRC_NAME:.c=.o)
OBJS = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

all: $(NAME)

$(NAME): $(LIBFT_DIR)/libft.a $(OBJS)
	@$(CC) -o $(NAME) $(CFLAGS) $(LIBS) $(OBJS) $(LIBFT_DIR)/libft.a
	@printf "%-100s\n" "$(NAME) compiled"

$(LIBFT_DIR)/libft.a:
	@make -C $(LIBFT_DIR)

$(OBJ_PATH)	:
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(addprefix $(OBJ_PATH)/,$(OBJ_DIRS))

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	@printf "%-200s\r" "$(CC) $(CFLAGS) -o $@"
	@$(CC) $(CFLAGS) -o $@ -c $<

init_submodules:
	git submodule update --init --recursive

update_submodules:
	git submodule update --recursive --remote

clean:
	rm -rf $(OBJ_PATH)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all, $(NAME), $(LIBFT_DIR)/libft.a, $(OBJ_PATH), $(OBJ_PATH), clean, fclean, re
