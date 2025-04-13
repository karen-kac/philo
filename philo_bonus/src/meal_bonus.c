/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meal_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 11:53:33 by myokono           #+#    #+#             */
/*   Updated: 2025/04/13 12:04:32 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	*meal_checker_routine(void *arg)
{
	t_shared	*shared;
	int			total_meals;

	shared = (t_shared *)arg;
	total_meals = 0;
	while (total_meals < shared->num_philos)
	{
		sem_wait(shared->meal_complete_sem);
		total_meals++;
	}
	kill_all_processes(shared);
	exit(0);
	return (NULL);
}

int	setup_meal_checker(t_shared *shared)
{
	pthread_t	meal_checker;

	if (shared->must_eat_count == -1)
		return (TRUE);
	if (pthread_create(&meal_checker, NULL, &meal_checker_routine, shared) != 0)
	{
		printf("%s\n", ERR_THREAD);
		return (FALSE);
	}
	pthread_detach(meal_checker);
	return (TRUE);
}
