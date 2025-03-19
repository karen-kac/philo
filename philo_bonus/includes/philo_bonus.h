/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claude            #+#  +:+       #+#        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/03/19 00:00:00 by claude           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <semaphore.h>
# include <signal.h>
# include <pthread.h>
# include <limits.h>

# define SEM_FORKS "/philo_forks"
# define SEM_PRINT "/philo_print"
# define SEM_FINISH "/philo_finish"

# define TRUE 1
# define FALSE 0

/* Philosophers state messages */
# define FORK_MSG "has taken a fork"
# define EAT_MSG "is eating"
# define SLEEP_MSG "is sleeping"
# define THINK_MSG "is thinking"
# define DIED_MSG "died"

/* Error messages */
# define ERR_ARGS "Error: Invalid number of arguments"
# define ERR_NUM_RANGE "Error: Numbers must be positive and within range"
# define ERR_SEM_OPEN "Error: Failed to open semaphore"
# define ERR_SEM_UNLINK "Error: Failed to unlink semaphore"
# define ERR_FORK "Error: Failed to fork process"
# define ERR_THREAD "Error: Failed to create thread"
# define ERR_MEMORY "Error: Memory allocation failed"

/* Shared data structure for all philosophers */
typedef struct s_shared
{
	int				num_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				must_eat_count;
	long long		start_time;
	sem_t			*forks_sem;
	sem_t			*print_sem;
	sem_t			*finish_sem;
	pid_t			*pids;
}	t_shared;

/* Individual philosopher data structure */
typedef struct s_philo
{
	int				id;
	long long		last_meal_time;
	int				eat_count;
	t_shared		*shared;
}	t_philo;

/* Utility functions */
int			ft_atoi(const char *str);
long long	get_current_time(void);
void		precise_sleep(long long ms);
void		safe_print(t_philo *philo, char *message);

/* Initialization functions */
int			parse_arguments(int argc, char **argv, t_shared *shared);
int			init_semaphores(t_shared *shared);
int			init_philosophers(t_shared *shared);

/* Philosopher routine functions */
void		philosopher_routine(t_philo *philo);
void		*monitor_death(void *arg);
void		take_forks(t_philo *philo);
void		eat(t_philo *philo);
void		release_forks(t_philo *philo);
void		sleep_and_think(t_philo *philo);

/* Cleanup functions */
void		cleanup_semaphores(t_shared *shared);
void		kill_all_processes(t_shared *shared);

#endif