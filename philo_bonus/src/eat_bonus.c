/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:00:00 by myokono           #+#    #+#             */
/*   Updated: 2025/03/10 15:41:20 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

static void	philo_one(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	sem_wait(data->forks_sem);
	print_status(data, philo->id, "has taken a fork");
	ft_usleep(data->shared->time_to_die * 2);
	sem_post(data->forks_sem);
	return ;
}

void	philo_eat(t_philo *philo)
{
	t_data	*data;
	int		index;

	data = philo->data;
	index = philo->id - 1;
	if (data->shared->nb_philo == 1)
	{
		philo_one(philo);
		return ;
	}
	sem_wait(data->forks_sem);
	print_status(data, philo->id, "has taken a fork");
	sem_wait(data->forks_sem);
	print_status(data, philo->id, "has taken a fork");
	sem_wait(philo->meal_check_sem);
	print_status(data, philo->id, "is eating");
	philo->shared->philosophers[index].last_meal_time = \
		get_time_since_start(data);
	philo->shared->philosophers[index].eat_count++;
	sem_post(philo->meal_check_sem);
	ft_usleep(data->shared->time_to_eat);
	sem_post(data->forks_sem);
	sem_post(data->forks_sem);
}
