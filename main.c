/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apayet <apayet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 22:57:34 by apayet            #+#    #+#             */
/*   Updated: 2023/07/12 23:37:51 by apayet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <stdio.h>

/**
 * @brief 
 * 
 */
static pid_t	*alloc_pid_array(int nbr_of_child_process)
{
	pid_t	*pid_array;

	pid_array = malloc(sizeof(pid_t) * nbr_of_child_process);
	if (pid_array == ((pid_t *)0))
	{
		printf("Error append\n");
		exit(1);
	}
	memset(pid_array, 0, sizeof(pid_t) * nbr_of_child_process);
	return (pid_array);
}

/**
 * @brief The child do something and exit 0 if sucess 
 * 			need to use exit() in this case
 * 			free also memory inside the child 
 * @param id_of_the_child 
 */
static void	child_do_something(pid_t *pid_array, int id_of_the_child)
{
	printf("I'm the child #%d\n", id_of_the_child);
	free(pid_array);
	pid_array = (pid_t *)0;
	exit(0);
}

/**
 * @brief Create fork, child is determined if pid is equal to 0
 * 			the main process (parent) will not execute child_do_something
 * @param pid_array 
 * @param nbr_of_child 
 */
static void	create_fork_process(pid_t *pid_array, int nbr_of_child)
{
	int	i;

	i = 0;
	while (i < nbr_of_child)
	{
		pid_array[i] = fork();
		if (pid_array[i] < 0)
		{
			printf("Fork error\n");
			free(pid_array);
			pid_array = (pid_t *)0;
			exit(1);
		}
		if (pid_array[i] == 0)
			child_do_something(pid_array, i);
		i++;
	}
}

static int	wait_for_syncing_processes(pid_t *pid_array, int nbr_of_child)
{
	int	i;
	int	exit_status_child;

	i = 0;
	printf("I'm the parent, I'm waiting my child process\n");
	while (i < nbr_of_child)
	{
		waitpid(pid_array[i], &exit_status_child, 0);
		i++;
	}
	printf("Done, all child process synced to the main process\n");
	return (exit_status_child);
}

int	main(void)
{
	pid_t	*pid_array;
	int		nbr_of_child;
	int		last_child_status;

	pid_array = (pid_t *)0;
	nbr_of_child = 2;
	pid_array = alloc_pid_array(nbr_of_child);
	create_fork_process(pid_array, nbr_of_child);
	last_child_status = wait_for_syncing_processes(pid_array, nbr_of_child);
	free(pid_array);
	pid_array = (pid_t *)0;
	return (last_child_status);
}
