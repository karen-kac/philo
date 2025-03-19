/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:34:21 by myokono           #+#    #+#             */
/*   Updated: 2025/03/19 21:12:59 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

static void	cleanup_all(t_shared *shared)
{
	cleanup_semaphores(shared);
	if (shared->pids)
		free(shared->pids);
}

static void	init_philo_and_run(t_shared *shared, int i)
{
	t_philo	philo;

	philo.id = i;
	philo.last_meal_time = shared->start_time;
	philo.eat_count = 0;
	philo.shared = shared;
	philosopher_routine(&philo);
	exit(EXIT_SUCCESS);
}

static int	create_philosophers(t_shared *shared)
{
	int	i;
	int	pid;

	i = 0;
	shared->start_time = get_current_time();
	while (i < shared->num_philos)
	{
		pid = fork();
		if (pid < 0)
		{
			printf("%s\n", ERR_FORK);
			kill_all_processes(shared);
			return (FALSE);
		}
		else if (pid == 0)
			init_philo_and_run(shared, i);
		else
			shared->pids[i] = pid;
		i++;
	}
	return (TRUE);
}

static void	wait_for_philosophers(t_shared *shared)
{
	int	i;
	int	status;
	int	dead_philo_found;
	int	exit_status;

	dead_philo_found = FALSE;
	i = 0;
	while (i < shared->num_philos)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
		{
			exit_status = WEXITSTATUS(status);
			if (exit_status == 1)
			{
				dead_philo_found = TRUE;
				break ;
			}
		}
		i++;
	}
	if (dead_philo_found)
		kill_all_processes(shared);
}

int	main(int argc, char **argv)
{
	t_shared	shared;

	memset(&shared, 0, sizeof(t_shared));
	shared.must_eat_count = -1;
	if (!parse_arguments(argc, argv, &shared))
		return (1);
	shared.pids = malloc(sizeof(pid_t) * shared.num_philos);
	if (!shared.pids)
	{
		printf("%s\n", ERR_MEMORY);
		return (1);
	}
	if (!init_semaphores(&shared))
	{
		free(shared.pids);
		return (1);
	}
	if (!create_philosophers(&shared))
	{
		cleanup_all(&shared);
		return (1);
	}
	wait_for_philosophers(&shared);
	cleanup_all(&shared);
	return (0);
}
