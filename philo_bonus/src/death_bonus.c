/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:53:36 by myokono           #+#    #+#             */
/*   Updated: 2025/03/24 17:48:44 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	*monitor_death(void *arg)
{
	t_philo		*philo;
	long long	current_time;
	long long	last_meal_time;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->shared->meal_check_sem);
		last_meal_time = philo->last_meal_time;
		sem_post(philo->shared->meal_check_sem);
		current_time = get_current_time();
		if ((current_time - last_meal_time) > philo->shared->time_to_die)
		{
			safe_print(philo, DIED_MSG);
			exit(1);
		}
		usleep(1000);
	}
	return (NULL);
}
