# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/19 20:43:14 by myokono           #+#    #+#              #
#    Updated: 2025/04/13 11:57:21 by myokono          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = src/main_bonus.c src/utils_bonus.c src/philosopher_bonus.c \
		src/clean_bonus.c src/death_bonus.c src/init_bonus.c src/routine_bonus.c \
		src/meal_bonus.c
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