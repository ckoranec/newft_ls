NAME = ft_ls

SRC = parseandstuff.c print2.c  print.c main.c ls_mergesort.c

OBJ = $(SRC:.c=.o)

SRC_PATH = src/

SRC_POS = $(addprefix $(SRC_PATH),$(SRC))

INC = -I includes

CC = gcc

FLAGS = -Wall -Wextra -Werror -g #-fsanitize=address
all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	@gcc $(FLAGS) $(INC) $(OBJ) libft/libft.a -o $(NAME)

$(OBJ):
	@$(CC) $(FLAGS) -c $(SRC_POS)

clean:
	@make clean -C libft
	@rm -f $(OBJ)

fclean: clean
	@make fclean -C libft
	@rm -f $(NAME)

re: fclean all

.PHONY : all, re, clean, fclean