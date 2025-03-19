/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:43:28 by myokono           #+#    #+#             */
/*   Updated: 2025/03/19 20:58:51 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	ft_atoi(const char *str)
{
	int	res;
	int	sign;
	int	i;

	res = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_sleep(long long ms)
{
	long long	start_time;

	start_time = get_current_time();
	while ((get_current_time() - start_time) < ms)
		usleep(500);
}

void	safe_print(t_philo *philo, char *message)
{
	sem_wait(philo->shared->print_sem);
	printf("%lld %d %s\n", \
		get_current_time() - philo->shared->start_time, \
		philo->id + 1, message);
	if (message[0] != 'd')
		sem_post(philo->shared->print_sem);
}

/* Parse command line arguments */
int	parse_arguments(int argc, char **argv, t_shared *shared)
{
	if (argc < 5 || argc > 6)
	{
		printf("%s\n", ERR_ARGS);
		return (FALSE);
	}
	shared->num_philos = ft_atoi(argv[1]);
	shared->time_to_die = ft_atoi(argv[2]);
	shared->time_to_eat = ft_atoi(argv[3]);
	shared->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		shared->must_eat_count = ft_atoi(argv[5]);
	if (shared->num_philos <= 0 || shared->time_to_die <= 0 || \
		shared->time_to_eat <= 0 || shared->time_to_sleep <= 0 || \
		(argc == 6 && shared->must_eat_count <= 0))
	{
		printf("%s\n", ERR_NUM_RANGE);
		return (FALSE);
	}
	return (TRUE);
}
