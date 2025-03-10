/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:00:00 by myokono           #+#    #+#             */
/*   Updated: 2025/03/10 15:30:10 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	*check_death(void *arg)
{
	t_philo		*philo;
	t_data		*data;
	long long	time;
	int			index;

	philo = (t_philo *)arg;
	data = philo->data;
	index = philo->id - 1;
	while (1)
	{
		sem_wait(philo->meal_check_sem);
		time = get_time_since_start(data) - \
			philo->shared->philosophers[index].last_meal_time;
		if (time > data->shared->time_to_die)
		{
			sem_wait(data->print_sem);
			printf("%lld %d died\n", get_time_since_start(data), philo->id);
			sem_post(data->finish_sem);
			exit(EXIT_DEATH);
		}
		sem_post(philo->meal_check_sem);
		ft_usleep(1);
	}
	return (NULL);
}

void	kill_processes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->shared->nb_philo)
	{
		if (data->philos[i].pid > 0)
			kill(data->philos[i].pid, SIGKILL);
		i++;
	}
}
