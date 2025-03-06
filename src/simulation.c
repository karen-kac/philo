/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:17:41 by myokono           #+#    #+#             */
/*   Updated: 2025/03/06 15:11:55 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	check_if_all_ate(t_data *data)
{
	int	i;
	int	finished;

	if (data->must_eat == -1)
		return ;
	i = 0;
	finished = 0;
	while (i < data->nb_philo)
	{
		if (data->philos[i].eat_count >= data->must_eat)
			finished++;
		i++;
	}
	if (finished == data->nb_philo)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->all_ate = true;
		pthread_mutex_unlock(&data->death_mutex);
	}
	return ;
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
		pthread_mutex_lock(&data->meal_mutex);
		print_status(data, philo->id, "is sleeping");
		pthread_mutex_unlock(&data->meal_mutex);
		ft_usleep(data->time_to_sleep);
		if (is_simulation_over(data))
			break ;
		pthread_mutex_lock(&data->meal_mutex);
		print_status(data, philo->id, "is thinking");
		pthread_mutex_unlock(&data->meal_mutex);
	}
	return (NULL);
}

bool	start_simulation(t_data *data)
{
	data->start_time = get_current_time();
	if (!create_threads(data))
		return (false);
	check_death(data, data->philos);
	return (join_threads(data));
}
