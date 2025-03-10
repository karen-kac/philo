/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:22:37 by myokono           #+#    #+#             */
/*   Updated: 2025/03/06 14:21:01 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	is_simulation_over(t_data *data)
{
	bool	result;

	pthread_mutex_lock(&data->death_mutex);
	result = (data->someone_died || data->all_ate);
	pthread_mutex_unlock(&data->death_mutex);
	return (result);
}

void	check_death(t_data *data, t_philo *philo)
{
	int			i;
	long long	time;

	while (!is_simulation_over(data))
	{
		i = 0;
		while (i < data->nb_philo && !is_simulation_over(data))
		{
			pthread_mutex_lock(&data->meal_mutex);
			time = get_time_since_start(data) - philo[i].last_meal_time;
			if (time > data->time_to_die)
			{
				pthread_mutex_lock(&data->death_mutex);
				data->someone_died = true;
				pthread_mutex_unlock(&data->death_mutex);
				pthread_mutex_lock(&data->print_mutex);
				printf("%lld %d died\n", \
					get_time_since_start(data), philo[i].id);
				pthread_mutex_unlock(&data->print_mutex);
			}
			pthread_mutex_unlock(&data->meal_mutex);
			i++;
		}
	}
}
