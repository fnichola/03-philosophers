/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 14:34:59 by fnichola          #+#    #+#             */
/*   Updated: 2022/03/03 17:28:08 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	print_error(char *message)
{
	printf("error\n");
	if (message)
		printf("%s\n", message);
	return (EXIT_FAILURE);
}

void	monitor_loop(t_data *data, t_philo **philos)
{
	while (1)
	{
		if (philo_monitor(data, philos))
			break ;
		usleep(1000);
	}
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philos;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat " \
			"time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	if (!init_data(argc, argv, &data))
		return (print_error("Must be fewer than 200 philosophers, " \
			"times must be 60ms or greater."));
	if (!alloc_memory(&data, &philos))
		return (print_error("Malloc failed."));
	if (!init_philos(&data, &philos))
		return (print_error("Failed to initialize philosphers."));
	if (!init_mutexes(&data, &philos))
		return (print_error("Failed to initalize mutexes."));
	if (!create_threads(&data, &philos))
		return (print_error("Failed while creating threads."));
	monitor_loop(&data, &philos);
	clean_up_threads(&data, &philos);
	return (EXIT_SUCCESS);
}
