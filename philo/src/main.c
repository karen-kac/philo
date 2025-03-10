/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:16:28 by myokono           #+#    #+#             */
/*   Updated: 2025/02/26 16:30:03 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 5 || argc > 6)
	{
		printf("Invalid number of arguments.");
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
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philos)
		return (false);
	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].eat_count = 0;
		data->philos[i].left_fork = i;
		data->philos[i].right_fork = (i + 1) % data->nb_philo;
		data->philos[i].last_meal_time = 0;
		data->philos[i].data = data;
		i++;
	}
	return (true);
}

static bool	init_data(t_data *data, int argc, char **argv)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->someone_died = false;
	data->all_ate = false;
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	if (data->nb_philo <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0 || (argc == 6 && data->must_eat <= 0))
	{
		printf("Error: Invalid arguments\n");
		return (false);
	}
	if (!init_mutex(data) || !init_philos(data))
	{
		clean_data(data);
		return (false);
	}
	return (true);
}

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
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
