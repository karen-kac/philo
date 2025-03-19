/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:34:07 by myokono           #+#    #+#             */
/*   Updated: 2025/03/19 19:34:12 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

/* 哲学者プロセスを作成する */
int	create_philosophers(t_data *data)
{
	int	i;
	int	pid;

	i = 0;
	while (i < data->philo_count)
	{
		pid = fork();
		if (pid == -1)
		{
			printf("Error: fork failed\n");
			/* 既に作成したプロセスを終了 */
			kill_processes(data);
			return (ERROR);
		}
		else if (pid == 0)
		{
			/* 子プロセス：哲学者のシミュレーションを実行 */
			philosopher_process(data, i + 1);
			exit(SUCCESS);
		}
		else
		{
			/* 親プロセス：子プロセスのPIDを保存 */
			data->pid_array[i] = pid;
		}
		i++;
	}
	return (SUCCESS);
}

/* 死亡監視スレッド */
void	*death_monitor(void *arg)
{
	t_philo		*philo;
	long long	current_time;

	philo = (t_philo *)arg;
	while (1)
	{
		current_time = get_time();
		/* 前回の食事から死亡時間が経過したかチェック */
		if (time_diff(philo->last_meal_time, current_time) > philo->data->time_to_die)
		{
			/* 死亡メッセージを表示して即座に終了 */
			sem_wait(philo->data->print_sem);
			printf("%lld %d %s\n", 
				time_diff(philo->data->start_time, current_time), 
				philo->id, 
				DIED_MSG);
			/* print_semを解放せずに終了（他の出力を防止） */
			exit(DEATH);
		}
		/* 必要回数の食事完了チェック */
		if (philo->data->must_eat_count > 0 
			&& philo->eat_count >= philo->data->must_eat_count)
		{
			/* 食事完了を通知してスレッド終了 */
			sem_post(philo->data->meal_check_sem);
			break ;
		}
		/* 監視間隔（短めに設定） */
		usleep(1000);
	}
	return (NULL);
}

/* フォークを取る */
void	take_forks(t_philo *philo)
{
	/* フォークセマフォから2本取得 */
	sem_wait(philo->data->forks_sem);
	print_status(philo, FORK_MSG, 0);
	sem_wait(philo->data->forks_sem);
	print_status(philo, FORK_MSG, 0);
}

/* 食事をする */
void	eat(t_philo *philo)
{
	/* 食事状態を表示 */
	print_status(philo, EAT_MSG, 0);
	
	/* 最後の食事時間を更新 */
	philo->last_meal_time = get_time();
	
	/* 食事時間だけ待機 */
	precise_sleep(philo->data->time_to_eat);
	
	/* 食事回数をカウントアップ */
	philo->eat_count++;
	
	/* フォークを返す */
	sem_post(philo->data->forks_sem);
	sem_post(philo->data->forks_sem);
}

/* 睡眠と思考 */
void	sleep_and_think(t_philo *philo)
{
	/* 睡眠状態を表示 */
	print_status(philo, SLEEP_MSG, 0);
	
	/* 睡眠時間だけ待機 */
	precise_sleep(philo->data->time_to_sleep);
	
	/* 思考状態を表示 */
	print_status(philo, THINK_MSG, 0);
}

/* 哲学者プロセスのメイン処理 */
void	philosopher_process(t_data *data, int id)
{
	t_philo	philo;

	/* 哲学者データの初期化 */
	init_philosopher(&philo, data, id);
	
	/* 死亡監視スレッドの作成 */
	if (pthread_create(&philo.monitor_thread, NULL, &death_monitor, &philo) != 0)
	{
		printf("Error: failed to create monitor thread\n");
		exit(ERROR);
	}
	pthread_detach(philo.monitor_thread);
	
	/* 偶数番号の哲学者は少し遅らせて開始（デッドロック防止） */
	if (id % 2 == 0)
		precise_sleep(data->time_to_eat / 2);
	
	/* 哲学者のライフサイクル */
	while (1)
	{
		take_forks(&philo);
		eat(&philo);
		sleep_and_think(&philo);
	}
}
