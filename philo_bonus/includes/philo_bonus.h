/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claude            #+#  +:+       #+#        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/03/19 00:00:00 by claude           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <signal.h>
# include <string.h>
# include <fcntl.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <pthread.h>
# include <stdbool.h>

/* セマフォ名 */
# define SEM_FORKS "/philo_forks"
# define SEM_PRINT "/philo_print"
# define SEM_MEAL_CHECK "/philo_meal_check"

/* メッセージ */
# define FORK_MSG "has taken a fork"
# define EAT_MSG "is eating"
# define SLEEP_MSG "is sleeping"
# define THINK_MSG "is thinking"
# define DIED_MSG "died"

/* エラーコード */
# define SUCCESS 0
# define ERROR 1
# define DEATH 2

typedef struct s_data
{
	int			philo_count;	/* 哲学者の数 */
	int			time_to_die;	/* 死亡までの時間（ミリ秒） */
	int			time_to_eat;	/* 食事時間（ミリ秒） */
	int			time_to_sleep;	/* 睡眠時間（ミリ秒） */
	int			must_eat_count;	/* 各哲学者が食べる必要のある回数（オプション） */
	long long	start_time;		/* シミュレーション開始時間 */
	pid_t		*pid_array;		/* 子プロセスPID配列 */
	sem_t		*forks_sem;		/* フォークセマフォ */
	sem_t		*print_sem;		/* 出力用セマフォ */
	sem_t		*meal_check_sem;/* 食事カウント用セマフォ */
}				t_data;

typedef struct s_philo
{
	int			id;				/* 哲学者ID (1から始まる) */
	int			eat_count;		/* 食事回数 */
	long long	last_meal_time;	/* 最後の食事時間 */
	t_data		*data;			/* 共有データ */
	pthread_t	monitor_thread;	/* 監視スレッド */
}				t_philo;

/* 初期化関数 */
int			init_data(t_data *data, int argc, char **argv);
int			init_semaphores(t_data *data);
void		init_philosopher(t_philo *philo, t_data *data, int id);

/* プロセス関連関数 */
int			create_philosophers(t_data *data);
void		philosopher_process(t_data *data, int id);
void		*death_monitor(void *arg);
void		*meal_counter(void *arg);

/* 哲学者の行動関数 */
void		take_forks(t_philo *philo);
void		eat(t_philo *philo);
void		sleep_and_think(t_philo *philo);

/* ユーティリティ関数 */
int			ft_atoi(const char *str);
long long	get_time(void);
long long	time_diff(long long past, long long present);
void		print_status(t_philo *philo, char *status, int is_dead);
void		precise_sleep(long long duration);

/* リソース管理関数 */
void		cleanup_resources(t_data *data);
void		close_semaphores(t_data *data);
void		unlink_semaphores(void);
void		kill_processes(t_data *data);

#endif