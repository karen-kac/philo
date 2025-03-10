/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:00:00 by myokono           #+#    #+#             */
/*   Updated: 2025/03/10 15:40:58 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	ft_atoi(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
		if (result > 2147483647 && sign == 1)
			return (-1);
		if (result > 2147483648 && sign == -1)
			return (0);
	}
	return (result * sign);
}

void	ft_usleep(long long time_in_ms)
{
	long long	start;

	start = get_current_time();
	while (get_current_time() - start < time_in_ms)
		usleep(100);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	clean_data(t_data *data)
{
	kill_processes(data);
	if (data->philos)
		free(data->philos);
	close_semaphores(data);
	unlink_semaphores();
	if (data->shared)
		munmap(data->shared, sizeof(t_data_shared));
}

void	print_status(t_data *data, int id, char *status)
{
	sem_wait(data->print_sem);
	printf("%lld %d %s\n", get_time_since_start(data), id, status);
	sem_post(data->print_sem);
}
