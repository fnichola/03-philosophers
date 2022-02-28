/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 14:34:59 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/28 15:06:26 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
		return (EXIT_FAILURE);
	if (!alloc_memory(&data, &philos))
		return (EXIT_FAILURE);
	if (!init_philos(&data, &philos))
		return (EXIT_FAILURE);
	if (!init_mutexes(&data, &philos))
		return (EXIT_FAILURE);
	if (!create_threads(&data, &philos))
		return (EXIT_FAILURE);
	monitor_loop(&data, &philos);
	clean_up_threads(&data, &philos);
	return (EXIT_SUCCESS);
}
