/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:17:41 by myokono           #+#    #+#             */
/*   Updated: 2025/02/26 17:48:29 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	check_if_all_ate(t_data *data)
{
	int	i;
	int	finished;

	if (data->must_eat == -1)
		return (false);
	i = 0;
	finished = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_lock(&data->meal_mutex);
		if (data->philos[i].eat_count >= data->must_eat)
			finished++;
		pthread_mutex_unlock(&data->meal_mutex);
		i++;
	}
	if (finished == data->nb_philo)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->all_ate = true;
		pthread_mutex_unlock(&data->death_mutex);
		return (true);
	}
	return (false);
}

static void	philo_eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->forks[philo->left_fork]);
	print_status(data, philo->id, "has taken a fork");
	if (data->nb_philo == 1)
	{
		ft_usleep(data->time_to_die * 2);
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
		return ;
	}
	pthread_mutex_lock(&data->forks[philo->right_fork]);
	print_status(data, philo->id, "has taken a fork");
	print_status(data, philo->id, "is eating");
	pthread_mutex_lock(&data->meal_mutex);
	philo->last_meal_time = get_time_since_start(data);
	philo->eat_count++;
	pthread_mutex_unlock(&data->meal_mutex);
	ft_usleep(data->time_to_eat);
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		ft_usleep(data->time_to_eat / 2);
	while (!is_simulation_over(data))
	{
		philo_eat(philo);
		if (is_simulation_over(data))
			break ;
		print_status(data, philo->id, "is sleeping");
		ft_usleep(data->time_to_sleep);
		if (is_simulation_over(data))
			break ;
		print_status(data, philo->id, "is thinking");
	}
	return (NULL);
}

bool	start_simulation(t_data *data)
{
	int	i;

	data->start_time = get_current_time();
	if (!create_threads(data))
		return (false);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_lock(&data->meal_mutex);
		data->philos[i].last_meal_time = 0;
		pthread_mutex_unlock(&data->meal_mutex);
		i++;
	}
	check_death(data, data->philos);
	return (join_threads(data));
}
