/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:17:12 by myokono           #+#    #+#             */
/*   Updated: 2025/03/06 15:26:51 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		if (result * sign > 2147483647 || result * sign < -2147483648)
			return (0);
		i++;
	}
	return (result * sign);
}

void	ft_usleep(long long time_in_ms)
{
	long long	start;

	start = get_current_time();
	while ((get_current_time() - start) < time_in_ms)
		usleep(100);
}

void	print_status(t_data *data, int id, char *status)
{
	pthread_mutex_lock(&data->print_mutex);
	pthread_mutex_lock(&data->death_mutex);
	if (data->someone_died == false && data->all_ate == false)
	{
		printf("%lld %d %s\n", get_time_since_start(data), id, status);
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_unlock(&data->print_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->death_mutex);
	pthread_mutex_unlock(&data->print_mutex);
}

void	clean_data(t_data *data)
{
	if (data->forks)
	{
		destroy_mutex(data);
		free(data->forks);
	}
	if (data->philos)
		free(data->philos);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
