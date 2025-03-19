/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:34:01 by myokono           #+#    #+#             */
/*   Updated: 2025/03/19 19:34:02 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/philo_bonus.h"

/* 現在時刻をミリ秒で取得 */
long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/* 2つの時刻の差分を計算 */
long long	time_diff(long long past, long long present)
{
	return (present - past);
}

/* 状態メッセージの表示 */
void	print_status(t_philo *philo, char *status, int is_dead)
{
	long long	current_time;

	(void)is_dead;
	/* 現在時刻を取得 */
	current_time = get_time();
	
	/* セマフォを獲得（死亡時の処理はdeath_monitorに移動） */
	sem_wait(philo->data->print_sem);
	
	/* 状態メッセージを表示 */
	printf("%lld %d %s\n", 
		time_diff(philo->data->start_time, current_time), 
		philo->id, 
		status);
	
	/* セマフォを解放 */
	sem_post(philo->data->print_sem);
}

/* 正確な待機時間を実現するための関数 */
void	precise_sleep(long long duration)
{
	long long	start_time;
	long long	current_time;

	start_time = get_time();
	while (1)
	{
		current_time = get_time();
		if (time_diff(start_time, current_time) >= duration)
			break ;
		usleep(100);  /* 短い間隔でチェックして精度を向上 */
	}
}

/* セマフォのアンリンク */
void	unlink_semaphores(void)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_MEAL_CHECK);
}

/* セマフォのクローズ */
void	close_semaphores(t_data *data)
{
	if (data->forks_sem)
		sem_close(data->forks_sem);
	if (data->print_sem)
		sem_close(data->print_sem);
	if (data->meal_check_sem)
		sem_close(data->meal_check_sem);
}

/* 全プロセスの強制終了 */
void	kill_processes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (data->pid_array[i] > 0)
		{
			kill(data->pid_array[i], SIGKILL);  /* SIGTERMよりも確実に終了させるためSIGKILLを使用 */
		}
		i++;
	}
	/* プロセス終了を確実にするため短い待機 */
	usleep(1000);
}

/* リソースの解放 */
void	cleanup_resources(t_data *data)
{
	/* セマフォのクローズ */
	close_semaphores(data);
	
	/* セマフォのアンリンク */
	unlink_semaphores();
	
	/* メモリの解放 */
	if (data->pid_array)
		free(data->pid_array);
}