# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/20 14:34:30 by pabmart2          #+#    #+#              #
#    Updated: 2025/05/26 21:02:43 by pabmart2         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fno-inline
LDFLAGS = -lm
BUILD_DIR = build
BONUS_BUILD_DIR = build_bonus
OBJ_DIR = build/obj
BONUS_OBJ_DIR = build_bonus/obj
NAME = pipex

BONUS_SRC = \
	bonus/src_bonus/cmd_resolver_bonus.c \
	bonus/src_bonus/execution_bonus.c \
	bonus/src_bonus/file_manager_bonus.c \
	bonus/src_bonus/fork_bonus.c \
	bonus/src_bonus/heredoc_bonus.c \
	bonus/src_bonus/main_bonus.c \
	bonus/src_bonus/pinfo_bonus.c \
	bonus/src_bonus/utils_bonus.c \

BONUS_OBJ = $(addprefix $(BONUS_OBJ_DIR)/, $(BONUS_SRC:.c=.o))

SRC = \
	src/cmd_resolver.c \
	src/execution.c \
	src/file_manager.c \
	src/fork.c \
	src/main.c \
	src/utils.c \

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

LIBS = \
	lib/libft/include/libft.a

INCLUDES = \
	-Iinclude \
	-Ilib/libft/include

BONUS_INCLUDES = \
	-Ibonus/include_bonus \
	-Ilib/libft/include

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR) $(BONUS_OBJ_DIR)
	@echo "\033[31mObject files removed\033[0m"

fclean: clean
	@rm -f $(BUILD_DIR)/$(NAME)
	@rm -f $(BONUS_BUILD_DIR)/$(NAME)
	@$(MAKE) -C lib/libft fclean
	@echo "\033[31m$(NAME) removed\033[0m"

re: fclean
	$(MAKE) all

libft:
	@echo "\033[33mCompiling libft...\033[0m"
	@$(MAKE) -C lib/libft

bonus: libft $(BONUS_OBJ)
	@mkdir -p $(BONUS_BUILD_DIR)
	@$(CC) $(BONUS_OBJ) -o $(BONUS_BUILD_DIR)/$(NAME) $(LIBS) $(LDFLAGS) $(BONUS_INCLUDES)
	@echo "\033[32m\n¡$(NAME) compiled! \
	ᕦ(\033[36m⌐■\033[32m_\033[36m■\033[32m)ᕤ\n"

$(BONUS_OBJ): $(BONUS_OBJ_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(BONUS_INCLUDES) -c $< -o $@
	@echo "\033[34mCompiling: \033[0m$<"

$(NAME): libft $(OBJ)
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(OBJ) -o $(BUILD_DIR)/$(NAME) $(LIBS) $(LDFLAGS) $(INCLUDES)
	@echo "\033[32m\n¡$(NAME) compiled! \
	ᕦ(\033[36m⌐■\033[32m_\033[36m■\033[32m)ᕤ\n"

$(OBJ): $(OBJ_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "\033[34mCompiling: \033[0m$<"

.PHONY: all clean fclean re bonus
