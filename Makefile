NAME = pipex
INC = pipex.h

SRCS += pipex.c
SRCS += pipex_utils.c

OBJS_DIR = obj/
OBJS = $(addprefix $(OBJS_DIR),$(SRCS:.c=.o))

CC = gcc -Wall -Wextra -Werror

LFT_DIR = ./libft/
LFT_LIB = $(LFT_DIR)libft.a
LFT_LNK = -L $(LFT_DIR) -l ft

all: $(NAME)

$(NAME) : $(OBJS) $(LFT_LIB)
	$(CC) $(OBJS) $(LFT_LNK) -lm -o $(NAME)

$(OBJS): $(OBJS_DIR)%.o: %.c $(OBJS_DIR)
	$(CC) -I $(LFT_DIR) -c $< -o $@

$(OBJS_DIR):
	mkdir $@

$(LFT_LIB):
	$(MAKE) -C $(LFT_DIR)

bonus : all

clean:
	rm -rf $(OBJS)
	$(MAKE) clean -C $(LFT_DIR)

fclean: clean
	rm -rf $(NAME)
	$(MAKE) fclean -C $(LFT_DIR)

re: fclean all

.PHONY: all clean fclean re
