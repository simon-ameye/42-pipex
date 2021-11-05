NAME = pipex
INC = pipex.h

SRCS += pipex.c
SRCS += pipex_utils.c
SRCS += pipex_tabs.c
SRCS += pipex_print.c
SRCS += ft_substr.c
SRCS += ft_split.c
SRCS += ft_strnstr.c
SRCS += ft_strjoin.c

OBJS_DIR = obj
OBJS = $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))
CC = gcc -Wall -Wextra -Werror

all : $(OBJS_DIR) $(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(NAME) : $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJS_DIR)/%.o : %.c
	@echo "compiling $<"
	@$(CC) -c $< -o $@

bonus : all

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re