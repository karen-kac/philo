/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_meals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:45:33 by myokono           #+#    #+#             */
/*   Updated: 2025/03/10 15:45:48 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

static void	all_ate(t_data *data)
{
	sem_wait(data->print_sem);
	sem_post(data->finish_sem);
	return ;
}

void	*monitor_meals(void *arg)
{
	t_data	*data;
	int		i;
	int		finished;

	data = (t_data *)arg;
	if (data->shared->must_eat == -1)
		return (NULL);
	while (1)
	{
		finished = 0;
		i = 0;
		while (i < data->shared->nb_philo)
		{
			if (data->shared->philosophers[i].eat_count >= \
				data->shared->must_eat)
				finished++;
			i++;
		}
		if (finished == data->shared->nb_philo)
		{
			all_ate(data);
			return (NULL);
		}
	}
	return (NULL);
}
