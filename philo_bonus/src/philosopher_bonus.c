/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:34:07 by myokono           #+#    #+#             */
/*   Updated: 2025/03/24 17:45:55 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	take_forks(t_philo *philo)
{
	sem_wait(philo->shared->forks_sem);
	safe_print(philo, FORK_MSG);
	sem_wait(philo->shared->forks_sem);
	safe_print(philo, FORK_MSG);
}

void	eat(t_philo *philo)
{
	sem_wait(philo->shared->meal_check_sem);
	philo->last_meal_time = get_current_time();
	sem_post(philo->shared->meal_check_sem);
	safe_print(philo, EAT_MSG);
	precise_sleep(philo->shared->time_to_eat);
	philo->eat_count++;
}

void	release_forks(t_philo *philo)
{
	sem_post(philo->shared->forks_sem);
	sem_post(philo->shared->forks_sem);
}

void	sleep_and_think(t_philo *philo)
{
	safe_print(philo, SLEEP_MSG);
	precise_sleep(philo->shared->time_to_sleep);
	safe_print(philo, THINK_MSG);
}
