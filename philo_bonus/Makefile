# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/19 19:33:55 by myokono           #+#    #+#              #
#    Updated: 2025/03/19 19:33:56 by myokono          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME		= philo_bonus

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -f

SRCS		= src/main_bonus.c src/init_bonus.c src/philosopher_bonus.c src/utils_bonus.c src/meal_counter_bonus.c
OBJS		= $(SRCS:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

bonus:		all

.PHONY:		all clean fclean re bonus