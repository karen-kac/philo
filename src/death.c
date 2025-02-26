/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:22:37 by myokono           #+#    #+#             */
/*   Updated: 2025/02/26 17:50:07 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	is_simulation_over(t_data *data)
{
	bool	result;

	result = false;
	pthread_mutex_lock(&data->death_mutex);
	if (data->someone_died || data->all_ate)
		result = true;
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
				print_status(data, philo[i].id, "died");
				pthread_mutex_lock(&data->death_mutex);
				data->someone_died = true;
				pthread_mutex_unlock(&data->death_mutex);
			}
			pthread_mutex_unlock(&data->meal_mutex);
			i++;
		}
		if (check_if_all_ate(data))
			break ;
		usleep(1000);
	}
}
