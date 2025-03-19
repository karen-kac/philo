/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:34:21 by myokono           #+#    #+#             */
/*   Updated: 2025/03/19 20:03:52 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/philo_bonus.h"



/* Clean up all resources */
static void	cleanup_all(t_shared *shared)
{
	cleanup_semaphores(shared);
	if (shared->pids)
		free(shared->pids);
}

/* Create processes for each philosopher */
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
		{
			// Child process (philosopher)
			t_philo	philo;

			philo.id = i;
			philo.last_meal_time = shared->start_time;
			philo.eat_count = 0;
			philo.shared = shared;
			philosopher_routine(&philo);
			exit(0);
		}
		else
		{
			// Parent process
			shared->pids[i] = pid;
		}
		i++;
	}
	return (TRUE);
}

/* Wait for philosophers to finish or die */
static void	wait_for_philosophers(t_shared *shared)
{
	int	i;
	int	status;
	int	dead_philo_found;

	dead_philo_found = FALSE;
	for (i = 0; i < shared->num_philos; i++)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
		{
			int exit_status = WEXITSTATUS(status);
			if (exit_status == 1)
			{
				dead_philo_found = TRUE;
				break;
			}
		}
	}
	if (dead_philo_found)
		kill_all_processes(shared);
}

/* Main function */
int	main(int argc, char **argv)
{
	t_shared	shared;

	memset(&shared, 0, sizeof(t_shared));
	shared.must_eat_count = -1;
	
	// Parse arguments and initialize resources
	if (!parse_arguments(argc, argv, &shared))
		return (1);

	// Allocate memory for process IDs
	shared.pids = malloc(sizeof(pid_t) * shared.num_philos);
	if (!shared.pids)
	{
		printf("%s\n", ERR_MEMORY);
		return (1);
	}

	// Initialize semaphores
	if (!init_semaphores(&shared))
	{
		free(shared.pids);
		return (1);
	}

	// Create philosopher processes
	if (!create_philosophers(&shared))
	{
		cleanup_all(&shared);
		return (1);
	}

	// Wait for philosophers to finish or die
	wait_for_philosophers(&shared);

	// Clean up all resources
	cleanup_all(&shared);
	return (0);
}