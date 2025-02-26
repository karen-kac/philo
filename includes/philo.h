/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:16:09 by myokono           #+#    #+#             */
/*   Updated: 2025/02/26 14:20:40 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <string.h>

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define RESET "\033[0m"

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				eat_count;
	int				left_fork;
	int				right_fork;
	long long		last_meal_time;
	pthread_t		thread_id;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	bool			all_ate;
	bool			someone_died;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	death_mutex;
	t_philo			*philos;
}	t_data;

/* utils.c */
int			ft_atoi(const char *str);
void		ft_usleep(long long time_in_ms);
long long	get_time(void);
void		clean_data(t_data *data);
void		print_status(t_data *data, int id, char *status);

/* simulation.c */
bool		is_simulation_over(t_data *data);
void		check_death(t_data *data, t_philo *philo);
void		*philo_routine(void *arg);
bool		start_simulation(t_data *data);

/* thread_management.c */
bool		create_threads(t_data *data);
bool		join_threads(t_data *data);
bool		init_mutex(t_data *data);
void		destroy_mutex(t_data *data);

/* time_management.c */
long long	get_current_time(void);
void		set_start_time(t_data *data);
long long	time_diff(long long past, long long present);
long long	get_time_since_start(t_data *data);

#endif