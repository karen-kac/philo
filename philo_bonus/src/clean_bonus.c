/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:55:47 by myokono           #+#    #+#             */
/*   Updated: 2025/03/24 17:47:33 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

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
	if (shared->meal_check_sem != SEM_FAILED)
	{
		sem_close(shared->meal_check_sem);
		sem_unlink(SEM_MEAL_CHECK);
	}
}

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
