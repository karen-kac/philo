/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:19:31 by myokono           #+#    #+#             */
/*   Updated: 2025/02/26 16:21:39 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	set_start_time(t_data *data)
{
	data->start_time = get_current_time();
}

long long	time_diff(long long past, long long present)
{
	return (present - past);
}

long long	get_time_since_start(t_data *data)
{
	return (get_current_time() - data->start_time);
}
