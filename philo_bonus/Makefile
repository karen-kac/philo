# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/10 09:50:11 by myokono           #+#    #+#              #
#    Updated: 2025/03/10 15:46:38 by myokono          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

SRCDIR = src/
INCDIR = includes/
SRCFILES = main_bonus.c \
		utils_bonus.c \
		simulation_bonus.c \
		death_bonus.c \
		process_management_bonus.c \
		time_management_bonus.c \
		eat_bonus.c \
		monitor_meals_bonus.c

SRCS = $(addprefix $(SRCDIR), $(SRCFILES))
OBJS = $(SRCS:.c=.o)
INCLUDES = -I$(INCDIR)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re