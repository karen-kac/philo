/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:07:23 by myokono           #+#    #+#             */
/*   Updated: 2025/03/10 09:41:05 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	lock_first_fork(t_philo *philo, int *first_fork)
{
	t_data	*data;

	data = philo->data;
	if (philo->left_fork < philo->right_fork)
		*first_fork = philo->left_fork;
	else
		*first_fork = philo->right_fork;
	pthread_mutex_lock(&data->forks[*first_fork]);
	if (is_simulation_over(data))
	{
		pthread_mutex_unlock(&data->forks[*first_fork]);
		return (0);
	}
	return (1);
}

static int	lock_second_fork(t_philo *philo, int *second_fork, int first_fork)
{
	t_data	*data;

	data = philo->data;
	if (philo->left_fork != first_fork)
		*second_fork = philo->left_fork;
	else
		*second_fork = philo->right_fork;
	pthread_mutex_lock(&data->forks[*second_fork]);
	if (is_simulation_over(data))
	{
		pthread_mutex_unlock(&data->forks[first_fork]);
		pthread_mutex_unlock(&data->forks[*second_fork]);
		return (0);
	}
	return (1);
}

static void	perform_eating(t_philo *philo, int first_fork, int second_fork)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->meal_mutex);
	print_status(data, philo->id, "has taken a fork");
	pthread_mutex_unlock(&data->meal_mutex);
	pthread_mutex_lock(&data->meal_mutex);
	print_status(data, philo->id, "has taken a fork");
	print_status(data, philo->id, "is eating");
	philo->last_meal_time = get_time_since_start(data);
	philo->eat_count++;
	check_if_all_ate(data);
	pthread_mutex_unlock(&data->meal_mutex);
	ft_usleep(data->time_to_eat);
	pthread_mutex_unlock(&data->forks[first_fork]);
	pthread_mutex_unlock(&data->forks[second_fork]);
}

void	philo_eat(t_philo *philo)
{
	int		first_fork;
	int		second_fork;
	t_data	*data;

	data = philo->data;
	if (data->nb_philo == 1)
	{
		if (!lock_first_fork(philo, &first_fork))
			return ;
		pthread_mutex_lock(&data->meal_mutex);
		print_status(data, philo->id, "has taken a fork");
		pthread_mutex_unlock(&data->meal_mutex);
		ft_usleep(philo->data->time_to_die * 2);
		pthread_mutex_unlock(&data->forks[first_fork]);
		return ;
	}
	if (!lock_first_fork(philo, &first_fork))
		return ;
	if (!lock_second_fork(philo, &second_fork, first_fork))
		return ;
	perform_eating(philo, first_fork, second_fork);
}
