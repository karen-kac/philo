/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:58:54 by myokono           #+#    #+#             */
/*   Updated: 2025/04/13 12:58:01 by myokono          ###   ########.fr       */
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
	shared->forks_sem = SEM_FAILED;
	shared->print_sem = SEM_FAILED;
	shared->meal_check_sem = SEM_FAILED;
	shared->meal_complete_sem = SEM_FAILED;
	shared->init_complete_sem = SEM_FAILED;
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_MEAL_CHECK);
	sem_unlink(SEM_MEAL_COMPLETE);
	sem_unlink(SEM_INIT_COMPLETE);
	if (!init_single_sem(&shared->forks_sem, SEM_FORKS, shared->num_philos))
		return (FALSE);
	if (!init_single_sem(&shared->print_sem, SEM_PRINT, 1))
		return (cleanup_semaphores(shared), FALSE);
	if (!init_single_sem(&shared->meal_check_sem, SEM_MEAL_CHECK, 1))
		return (cleanup_semaphores(shared), FALSE);
	if (!init_single_sem(&shared->meal_complete_sem, SEM_MEAL_COMPLETE, 0))
		return (cleanup_semaphores(shared), FALSE);
	if (!init_single_sem(&shared->init_complete_sem, SEM_INIT_COMPLETE, 0))
		return (cleanup_semaphores(shared), FALSE);
	return (TRUE);
}
