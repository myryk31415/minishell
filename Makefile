# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile_cool                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/09 15:34:56 by padam             #+#    #+#              #
#    Updated: 2024/02/10 23:20:12 by padam            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
INCLUDES = -I./includes
LIBFT_DIR = libft

SRC_PATH = src
OBJ_PATH = obj

SRCS = main.c
OBJ_NAME = $(SRCS:.c=.o)
OBJS = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

all: $(NAME)

$(NAME): $(LIBFT_DIR)/libft.a $(OBJS)
	$(CC) -o $(NAME) $(CFLAGS) $(OBJS) $(LIBFT_DIR)/libft.a

$(LIBFT_DIR)/libft.a:
	make -C $(LIBFT_DIR)

$(OBJ_PATH)	:
	mkdir -p $(OBJ_PATH)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	printf "%-50s\r" "$(CC) $@"
	$(CC) $(CFLAGS) -o $@ -c $<

init:
	git submodule update --init --recursive

clean:
	rm -rf $(OBJ_PATH)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

bonus: all

.PHONY: all, $(LIBFT_DIR)/libft.a, $(OBJ_PATH), clean, fclean, re, bonus
