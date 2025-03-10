/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:00:00 by myokono           #+#    #+#             */
/*   Updated: 2025/03/10 15:45:58 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	philo_routine(t_philo *philo)
{
	t_data	*data;
	int		index;

	data = philo->data;
	index = philo->id - 1;
	philo->shared->philosophers[index].last_meal_time = \
		get_time_since_start(data);
	if (pthread_create(&philo->death_check_thread, \
		NULL, check_death, philo) != 0)
		exit(EXIT_FAILURE);
	pthread_detach(philo->death_check_thread);
	if (philo->id % 2 == 0)
		ft_usleep(data->shared->time_to_eat / 2);
	while (1)
	{
		philo_eat(philo);
		print_status(data, philo->id, "is sleeping");
		ft_usleep(data->shared->time_to_sleep);
		print_status(data, philo->id, "is thinking");
		ft_usleep(1);
	}
	exit(EXIT_SUCCESS);
}

static void	wait_for_philosopher_processes(t_data *data)
{
	int		i;
	int		status;

	i = 0;
	while (i < data->shared->nb_philo)
	{
		waitpid(-1, &status, 0);
		i++;
	}
}

static void	*wait_for_signal(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	sem_wait(data->finish_sem);
	kill_processes(data);
	return (NULL);
}

bool	start_simulation(t_data *data)
{
	pthread_t	signal_thread;
	pthread_t	meal_monitor;

	data->shared->start_time = get_current_time();
	if (!create_processes(data))
		return (false);
	if (pthread_create(&signal_thread, NULL, wait_for_signal, data) != 0)
		return (false);
	pthread_detach(signal_thread);
	if (data->shared->must_eat > 0)
	{
		if (pthread_create(&meal_monitor, NULL, monitor_meals, data) != 0)
			return (false);
		pthread_detach(meal_monitor);
	}
	wait_for_philosopher_processes(data);
	return (true);
}
