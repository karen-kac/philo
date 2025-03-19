/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:34:01 by myokono           #+#    #+#             */
/*   Updated: 2025/03/19 20:04:12 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/philo_bonus.h"



/* Custom atoi implementation */
int	ft_atoi(const char *str)
{
	int	res;
	int	sign;
	int	i;

	res = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

/* Get current time in milliseconds */
long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/* Sleep with precision using usleep */
void	precise_sleep(long long ms)
{
	long long	start_time;

	start_time = get_current_time();
	while ((get_current_time() - start_time) < ms)
		usleep(500);
}

/* Print message with semaphore protection */
void	safe_print(t_philo *philo, char *message)
{
	sem_wait(philo->shared->print_sem);
	printf("%lld %d %s\n", 
		get_current_time() - philo->shared->start_time, 
		philo->id + 1, 
		message);
	if (message[0] != 'd')  // Don't release semaphore if it's a death message
		sem_post(philo->shared->print_sem);
}

/* Parse command line arguments */
int	parse_arguments(int argc, char **argv, t_shared *shared)
{
	if (argc < 5 || argc > 6)
	{
		printf("%s\n", ERR_ARGS);
		return (FALSE);
	}
	
	shared->num_philos = ft_atoi(argv[1]);
	shared->time_to_die = ft_atoi(argv[2]);
	shared->time_to_eat = ft_atoi(argv[3]);
	shared->time_to_sleep = ft_atoi(argv[4]);
	
	if (argc == 6)
		shared->must_eat_count = ft_atoi(argv[5]);
	
	if (shared->num_philos <= 0 || shared->time_to_die <= 0 ||
		shared->time_to_eat <= 0 || shared->time_to_sleep <= 0 ||
		(argc == 6 && shared->must_eat_count <= 0))
	{
		printf("%s\n", ERR_NUM_RANGE);
		return (FALSE);
	}
	
	return (TRUE);
}

/* Initialize semaphores */
int	init_semaphores(t_shared *shared)
{
	// Unlink any existing semaphores with the same names
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_FINISH);
	
	// Open semaphores
	shared->forks_sem = sem_open(SEM_FORKS, O_CREAT, 0644, shared->num_philos);
	if (shared->forks_sem == SEM_FAILED)
	{
		printf("%s: %s\n", ERR_SEM_OPEN, SEM_FORKS);
		return (FALSE);
	}
	
	shared->print_sem = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	if (shared->print_sem == SEM_FAILED)
	{
		printf("%s: %s\n", ERR_SEM_OPEN, SEM_PRINT);
		sem_close(shared->forks_sem);
		sem_unlink(SEM_FORKS);
		return (FALSE);
	}
	
	shared->finish_sem = sem_open(SEM_FINISH, O_CREAT, 0644, 0);
	if (shared->finish_sem == SEM_FAILED)
	{
		printf("%s: %s\n", ERR_SEM_OPEN, SEM_FINISH);
		sem_close(shared->forks_sem);
		sem_close(shared->print_sem);
		sem_unlink(SEM_FORKS);
		sem_unlink(SEM_PRINT);
		return (FALSE);
	}
	
	return (TRUE);
}

/* Clean up semaphores */
void	cleanup_semaphores(t_shared *shared)
{
	if (shared->forks_sem != SEM_FAILED)
	{
		sem_close(shared->forks_sem);
		sem_unlink(SEM_FORKS);
	}
	
	if (shared->print_sem != SEM_FAILED)
	{
		sem_close(shared->print_sem);
		sem_unlink(SEM_PRINT);
	}
	
	if (shared->finish_sem != SEM_FAILED)
	{
		sem_close(shared->finish_sem);
		sem_unlink(SEM_FINISH);
	}
}

/* Kill all philosopher processes */
void	kill_all_processes(t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->num_philos)
	{
		if (shared->pids[i] > 0)
			kill(shared->pids[i], SIGTERM);
		i++;
	}
}