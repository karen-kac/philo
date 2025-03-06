# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/26 14:20:10 by myokono           #+#    #+#              #
#    Updated: 2025/03/06 15:09:51 by myokono          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = philo
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
SRCS    = src/main.c src/utils.c src/simulation.c \
		src/thread_management.c src/time_management.c \
		src/death.c src/eat.c
OBJS    = $(SRCS:.c=.o)
RM      = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re