/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:34:25 by myokono           #+#    #+#             */
/*   Updated: 2025/03/19 19:34:27 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

/* 数値文字列を整数に変換 */
int	ft_atoi(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
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
		result = result * 10 + (str[i] - '0');
		if ((sign == 1 && result > 2147483647)
			|| (sign == -1 && result > 2147483648))
			return (0);
		i++;
	}
	return ((int)(result * sign));
}

/* データ構造の初期化 */
int	init_data(t_data *data, int argc, char **argv)
{
	/* 0で初期化 */
	memset(data, 0, sizeof(t_data));

	/* 引数を解析 */
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = -1;

	/* 引数の値を検証 */
	if (data->philo_count <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (argc == 6 && data->must_eat_count <= 0))
	{
		printf("Error: Invalid argument values\n");
		return (ERROR);
	}

	/* プロセスID配列の割り当て */
	data->pid_array = malloc(sizeof(pid_t) * data->philo_count);
	if (!data->pid_array)
	{
		printf("Error: Memory allocation failed\n");
		return (ERROR);
	}

	/* 開始時間の記録 */
	data->start_time = get_time();
	return (SUCCESS);
}

/* セマフォの初期化 */
int	init_semaphores(t_data *data)
{
	/* フォークセマフォ - フォークの総数で初期化 */
	data->forks_sem = sem_open(SEM_FORKS, O_CREAT | O_EXCL, 0644,
			data->philo_count);
	if (data->forks_sem == SEM_FAILED)
	{
		printf("Error: Failed to create forks semaphore\n");
		return (ERROR);
	}

	/* 出力用セマフォ - 1で初期化（排他制御用） */
	data->print_sem = sem_open(SEM_PRINT, O_CREAT | O_EXCL, 0644, 1);
	if (data->print_sem == SEM_FAILED)
	{
		printf("Error: Failed to create print semaphore\n");
		sem_close(data->forks_sem);
		sem_unlink(SEM_FORKS);
		return (ERROR);
	}

	/* 食事カウント用セマフォ（オプション） - 0で初期化 */
	if (data->must_eat_count > 0)
	{
		data->meal_check_sem = sem_open(SEM_MEAL_CHECK, O_CREAT | O_EXCL,
				0644, 0);
		if (data->meal_check_sem == SEM_FAILED)
		{
			printf("Error: Failed to create meal check semaphore\n");
			sem_close(data->forks_sem);
			sem_close(data->print_sem);
			sem_unlink(SEM_FORKS);
			sem_unlink(SEM_PRINT);
			return (ERROR);
		}
	}
	return (SUCCESS);
}

/* 哲学者データの初期化 */
void	init_philosopher(t_philo *philo, t_data *data, int id)
{
	philo->id = id;
	philo->eat_count = 0;
	philo->last_meal_time = get_time();
	philo->data = data;
}