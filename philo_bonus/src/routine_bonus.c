/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:01:03 by myokono           #+#    #+#             */
/*   Updated: 2025/03/19 21:06:02 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

static void	start_monitor_thread(t_philo *philo)
{
	pthread_t	monitor_thread;

	if (pthread_create(&monitor_thread, NULL, &monitor_death, philo) != 0)
	{
		printf("%s\n", ERR_THREAD);
		printf("%s\n", ERR_THREAD);
		exit(1);
	}
	pthread_detach(monitor_thread);
}

/* 哲学者が1人だけの場合の特別処理を切り出し */
static void	handle_single_philo(t_philo *philo)
{
	sem_wait(philo->shared->forks_sem);
	safe_print(philo, FORK_MSG);
	precise_sleep(philo->shared->time_to_die);
	safe_print(philo, DIED_MSG);
	sem_post(philo->shared->forks_sem);
	exit(1);
}

void	philosopher_routine(t_philo *philo)
{
	start_monitor_thread(philo);
	if (philo->shared->num_philos == 1)
		handle_single_philo(philo);
	if (philo->id % 2 == 0)
		usleep(10000);
	while (1)
	{
		take_forks(philo);
		eat(philo);
		release_forks(philo);
		if (philo->shared->must_eat_count != -1
			&& philo->eat_count >= philo->shared->must_eat_count)
			exit(0);
		sleep_and_think(philo);
	}
}
