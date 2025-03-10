/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_management_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:00:00 by myokono           #+#    #+#             */
/*   Updated: 2025/03/10 15:31:32 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

t_data_shared	*create_shared_memory(int nb_philo)
{
	t_data_shared	*shared_data;
	size_t			size;
	int				i;

	size = sizeof(t_data_shared);
	shared_data = mmap(NULL, size, PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (shared_data == MAP_FAILED)
	{
		printf("Error: Failed to create shared memory\n");
		return (NULL);
	}
	memset(shared_data, 0, size);
	i = 0;
	while (i < nb_philo)
	{
		shared_data->philosophers[i].eat_count = 0;
		shared_data->philosophers[i].last_meal_time = 0;
		i++;
	}
	return (shared_data);
}

bool	create_processes(t_data *data)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < data->shared->nb_philo)
	{
		pid = fork();
		if (pid == -1)
		{
			printf("Error: Failed to create process\n");
			kill_processes(data);
			return (false);
		}
		else if (pid == 0)
		{
			data->philos[i].pid = getpid();
			philo_routine(&data->philos[i]);
			exit(EXIT_SUCCESS);
		}
		else
			data->philos[i].pid = pid;
		i++;
	}
	return (true);
}

bool	init_semaphores(t_data *data)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_MEAL);
	sem_unlink(SEM_FINISH);
	data->forks_sem = sem_open(SEM_FORKS, O_CREAT | O_EXCL, S_IRWXU,
			data->shared->nb_philo);
	data->print_sem = sem_open(SEM_PRINT, O_CREAT | O_EXCL, S_IRWXU, 1);
	data->meal_sem = sem_open(SEM_MEAL, O_CREAT | O_EXCL, S_IRWXU, 1);
	data->finish_sem = sem_open(SEM_FINISH, O_CREAT | O_EXCL, S_IRWXU, 0);
	if (data->forks_sem == SEM_FAILED || data->print_sem == SEM_FAILED
		|| data->meal_sem == SEM_FAILED || data->finish_sem == SEM_FAILED)
	{
		printf("Error: Failed to initialize semaphores\n");
		return (false);
	}
	return (true);
}

void	close_semaphores(t_data *data)
{
	int	i;

	if (data->forks_sem)
		sem_close(data->forks_sem);
	if (data->print_sem)
		sem_close(data->print_sem);
	if (data->meal_sem)
		sem_close(data->meal_sem);
	if (data->finish_sem)
		sem_close(data->finish_sem);
	if (data->philos)
	{
		i = 0;
		while (i < data->shared->nb_philo)
		{
			if (data->philos[i].meal_check_sem)
				sem_close(data->philos[i].meal_check_sem);
			sem_unlink(data->philos[i].meal_sem_name);
			i++;
		}
	}
}

void	unlink_semaphores(void)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_MEAL);
	sem_unlink(SEM_FINISH);
}
