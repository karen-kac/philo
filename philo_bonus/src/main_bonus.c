/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:00:00 by myokono           #+#    #+#             */
/*   Updated: 2025/03/10 22:15:06 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

static bool	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 5 || argc > 6)
	{
		printf("Invalid number of arguments.\n");
		return (false);
	}
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf("Error: Arguments must be positive integers\n");
				return (false);
			}
			j++;
		}
		i++;
	}
	return (true);
}

static bool	init_philos(t_data *data)
{
	int		i;

	data->philos = malloc(sizeof(t_philo) * data->shared->nb_philo);
	if (!data->philos)
		return (false);
	i = 0;
	while (i < data->shared->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		data->philos[i].shared = data->shared;
		sprintf(data->philos[i].meal_sem_name, "/sem_meal_%d", i + 1);
		sem_unlink(data->philos[i].meal_sem_name);
		data->philos[i].meal_check_sem = sem_open(data->philos[i].meal_sem_name,
				O_CREAT | O_EXCL, S_IRWXU, 1);
		if (data->philos[i].meal_check_sem == SEM_FAILED)
			return (false);
		i++;
	}
	return (true);
}

static bool	parse_arguments(int argc, char **argv, t_data *data)
{
	data->shared->nb_philo = ft_atoi(argv[1]);
	data->shared->time_to_die = ft_atoi(argv[2]);
	data->shared->time_to_eat = ft_atoi(argv[3]);
	data->shared->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->shared->must_eat = ft_atoi(argv[5]);
	else
		data->shared->must_eat = -1;
	if (data->shared->nb_philo <= 0 || data->shared->time_to_die <= 0
		|| data->shared->time_to_eat <= 0 || data->shared->time_to_sleep <= 0
		|| (argc == 6 && data->shared->must_eat <= 0))
		return (printf("Error: Invalid arguments\n"), false);
	return (true);
}

static bool	init_data(t_data *data, int argc, char **argv)
{
	data->shared = create_shared_memory(ft_atoi(argv[1]));
	if (!data->shared)
		return (false);
	if (!parse_arguments(argc, argv, data))
		return (false);
	if (!init_semaphores(data) || !init_philos(data))
		return (clean_data(data), false);
	return (true);
}

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	unlink_semaphores();
	if (!check_args(argc, argv))
		return (1);
	if (!init_data(&data, argc, argv))
		return (1);
	if (!start_simulation(&data))
	{
		clean_data(&data);
		return (1);
	}
	clean_data(&data);
	return (0);
}
