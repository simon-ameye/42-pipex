NAME = pipex
INC = pipex.h

SRCS += pipex.c
SRCS += pipex_utils.c
SRCS += pipex_utils2.c
SRCS += pipex_substr.c
SRCS += pipex_split.c
SRCS += pipex_strnstr.c

OBJS_DIR = obj
OBJS = $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))
CC = clang -Wall -Wextra -Werror

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