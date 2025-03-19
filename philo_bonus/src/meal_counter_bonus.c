/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meal_counter_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:34:15 by myokono           #+#    #+#             */
/*   Updated: 2025/03/19 19:34:16 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/philo_bonus.h"

/* 食事回数を監視する（オプション）*/
void	*meal_counter(void *arg)
{
	t_data	*data;
	// int		i;
	int		total_meals;

	data = (t_data *)arg;
	total_meals = 0;
	while (total_meals < data->philo_count)
	{
		sem_wait(data->meal_check_sem);
		total_meals++;
	}
	/* 全ての哲学者が必要な回数だけ食事したので、全プロセスを終了 */
	kill_processes(data);
	return (NULL);
}