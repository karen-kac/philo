/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:00:00 by myokono           #+#    #+#             */
/*   Updated: 2025/03/10 15:38:56 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <string.h>
# include <sys/wait.h>
# include <signal.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>

/* Semaphore names */
# define SEM_FORKS "/sem_forks"
# define SEM_PRINT "/sem_print"
# define SEM_MEAL "/sem_meal"
# define SEM_FINISH "/sem_finish"

/* Process exit codes */
# define EXIT_SUCCESS 0
# define EXIT_DEATH 1
# define EXIT_FULL 2

typedef struct s_shared_philo
{
	int			eat_count;
	long long	last_meal_time;
}	t_shared_philo;

typedef struct s_data_shared
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	long long		start_time;
	t_shared_philo	philosophers[200];
}	t_data_shared;

typedef struct s_philo
{
	int				id;
	pid_t			pid;
	pthread_t		death_check_thread;
	sem_t			*meal_check_sem;
	char			meal_sem_name[30];
	t_data_shared	*shared;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	sem_t			*forks_sem;
	sem_t			*print_sem;
	sem_t			*meal_sem;
	sem_t			*finish_sem;
	t_data_shared	*shared;
	t_philo			*philos;
}	t_data;

/* utils.c */
int				ft_atoi(const char *str);
void			ft_usleep(long long time_in_ms);
long long		get_time(void);
void			clean_data(t_data *data);
void			print_status(t_data *data, int id, char *status);

/* simulation.c */
void			philo_routine(t_philo *philo);
bool			start_simulation(t_data *data);
void			*monitor_meals(void *arg);

/* death.c */
void			*check_death(void *arg);
void			kill_processes(t_data *data);

/* process_management.c */
bool			create_processes(t_data *data);
bool			init_semaphores(t_data *data);
void			close_semaphores(t_data *data);
void			unlink_semaphores(void);
t_data_shared	*create_shared_memory(int nb_philo);

/* time_management.c */
long long		get_current_time(void);
void			set_start_time(t_data *data);
long long		time_diff(long long past, long long present);
long long		get_time_since_start(t_data *data);

/* eat.c */
void			philo_eat(t_philo *philo);

#endif