/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:34:21 by myokono           #+#    #+#             */
/*   Updated: 2025/03/19 19:34:22 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/philo_bonus.h"

/* 引数の検証 */
static int	validate_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc != 5 && argc != 6)
	{
		printf("Error: wrong number of arguments\n");
		printf("Usage: %s number_of_philosophers time_to_die time_to_eat ", argv[0]);
		printf("time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (ERROR);
	}
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf("Error: arguments must be positive numbers\n");
				return (ERROR);
			}
			j++;
		}
		i++;
	}
	return (SUCCESS);
}

/* 子プロセスの終了を監視する */
static void	wait_for_processes(t_data *data)
{
	int	status;
	int	exit_status;
	pid_t pid;

	/* 非ブロッキングで最初に終了したプロセスを待機 */
	pid = waitpid(-1, &status, 0);
	if (pid > 0 && WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		if (exit_status == DEATH)
		{
			/* 死亡を検出したら即座に全プロセスを終了 */
			kill_processes(data);
			
			/* 残りのプロセスの終了を待機（終了状態は無視） */
			while (waitpid(-1, NULL, 0) > 0)
				;
			return;
		}
	}

	/* 死亡でない通常終了の場合は全プロセスの終了を待機 */
	while (waitpid(-1, &status, 0) > 0)
		;
}

/* meal_counter.c に移動 */

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	meal_thread;

	/* 引数の検証 */
	if (validate_args(argc, argv) != SUCCESS)
		return (ERROR);

	/* セマフォのアンリンク（前回実行の残りがあれば） */
	unlink_semaphores();

	/* データの初期化 */
	if (init_data(&data, argc, argv) != SUCCESS)
		return (ERROR);

	/* セマフォの初期化 */
	if (init_semaphores(&data) != SUCCESS)
	{
		cleanup_resources(&data);
		return (ERROR);
	}

	/* 食事回数監視スレッドの作成（オプション） */
	if (data.must_eat_count > 0)
	{
		if (pthread_create(&meal_thread, NULL, &meal_counter, &data) != 0)
		{
			printf("Error: failed to create meal counter thread\n");
			cleanup_resources(&data);
			return (ERROR);
		}
		pthread_detach(meal_thread);
	}

	/* 哲学者プロセスの作成 */
	if (create_philosophers(&data) != SUCCESS)
	{
		cleanup_resources(&data);
		return (ERROR);
	}

	/* 各プロセスの終了を待機 */
	wait_for_processes(&data);

	/* リソースのクリーンアップ */
	cleanup_resources(&data);
	return (SUCCESS);
}