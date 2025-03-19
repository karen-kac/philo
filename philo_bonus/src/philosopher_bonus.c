/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:34:07 by myokono           #+#    #+#             */
/*   Updated: 2025/03/19 20:04:42 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"



/* Take forks from the semaphore */
void	take_forks(t_philo *philo)
{
	sem_wait(philo->shared->forks_sem);
	safe_print(philo, FORK_MSG);
	sem_wait(philo->shared->forks_sem);
	safe_print(philo, FORK_MSG);
}

/* Eating action */
void	eat(t_philo *philo)
{
	philo->last_meal_time = get_current_time();
	safe_print(philo, EAT_MSG);
	precise_sleep(philo->shared->time_to_eat);
	philo->eat_count++;
}

/* Release forks back to the semaphore */
void	release_forks(t_philo *philo)
{
	sem_post(philo->shared->forks_sem);
	sem_post(philo->shared->forks_sem);
}

/* Sleeping and thinking actions */
void	sleep_and_think(t_philo *philo)
{
	safe_print(philo, SLEEP_MSG);
	precise_sleep(philo->shared->time_to_sleep);
	safe_print(philo, THINK_MSG);
}

/* Monitor death thread function */
void	*monitor_death(void *arg)
{
	t_philo		*philo;
	long long	current_time;

	philo = (t_philo *)arg;
	while (1)
	{
		current_time = get_current_time();
		if ((current_time - philo->last_meal_time) > philo->shared->time_to_die)
		{
			safe_print(philo, DIED_MSG);
			exit(1);
		}
		usleep(1000);
	}
	return (NULL);
}

/* Main philosopher routine */
void	philosopher_routine(t_philo *philo)
{
	pthread_t	monitor_thread;

	// Create death monitoring thread
	if (pthread_create(&monitor_thread, NULL, &monitor_death, philo) != 0)
	{
		printf("%s\n", ERR_THREAD);
		exit(1);
	}
	pthread_detach(monitor_thread);

	// If there's only one philosopher, they will wait for forks forever and die
	if (philo->shared->num_philos == 1)
	{
		sem_wait(philo->shared->forks_sem);
		safe_print(philo, FORK_MSG);
		precise_sleep(philo->shared->time_to_die);
		safe_print(philo, DIED_MSG);
		sem_post(philo->shared->forks_sem);
		exit(1);
	}

	// Stagger philosophers' actions based on their ID to prevent deadlock
	if (philo->id % 2 == 0)
		usleep(10000);

	// Main philosopher cycle
	while (1)
	{
		take_forks(philo);
		eat(philo);
		release_forks(philo);

		// Check if we've reached the required number of meals
		if (philo->shared->must_eat_count != -1 && 
			philo->eat_count >= philo->shared->must_eat_count)
			exit(0);

		sleep_and_think(philo);
	}
}