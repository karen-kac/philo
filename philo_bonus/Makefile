# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: claude                                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/19                              #+#    #+#              #
#    Updated: 2025/03/19                             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = src/main_bonus.c src/utils_bonus.c src/philosopher_bonus.c
OBJS = $(SRCS:.c=.o)

HEADERS = philo_bonus.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus