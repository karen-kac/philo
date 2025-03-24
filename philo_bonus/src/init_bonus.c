/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:58:54 by myokono           #+#    #+#             */
/*   Updated: 2025/03/24 17:48:05 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

static int	init_single_sem(sem_t **sem, const char *name, int value)
{
	*sem = sem_open(name, O_CREAT, 0644, value);
	if (*sem == SEM_FAILED)
	{
		printf("%s: %s\n", ERR_SEM_OPEN, name);
		return (FALSE);
	}
	return (TRUE);
}

int	init_semaphores(t_shared *shared)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_MEAL_CHECK);
	if (!init_single_sem(&shared->forks_sem, SEM_FORKS, shared->num_philos))
		return (FALSE);
	if (!init_single_sem(&shared->print_sem, SEM_PRINT, 1))
	{
		sem_close(shared->forks_sem);
		sem_unlink(SEM_FORKS);
		return (FALSE);
	}
	if (!init_single_sem(&shared->meal_check_sem, SEM_MEAL_CHECK, 1))
	{
		sem_close(shared->forks_sem);
		sem_close(shared->print_sem);
		sem_unlink(SEM_FORKS);
		sem_unlink(SEM_PRINT);
		return (FALSE);
	}
	return (TRUE);
}
